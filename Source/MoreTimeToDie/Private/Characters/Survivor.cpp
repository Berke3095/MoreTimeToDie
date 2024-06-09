#include "Characters/Survivor.h"

#include "MyGameManager.h" 

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/SkeletalMeshSocket.h" 

#include "MyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

#include "Widgets/PortraitWidget.h"
#include "MyHUD.h"
#include "Harvestables/Harvestable.h"

#include "Animation/AnimMontage.h"
#include "Characters/SurvivorAnimInstance.h"

ASurvivor::ASurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Selectable");

	SetCharacterSettings();
	SetCapsuleComponent();
	SetSkeletalMeshComponent();
	SetCharacterMovement();
}

void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
	
	GetReferences();

	if (PortraitWidget)
	{
		PortraitWidget->SetSurvivorHud(Portrait, Name, this);
		PortraitWidget->AddToUnDrafted(this);
		PortraitButton->OnClicked.AddDynamic(this, &ASurvivor::OnPortraitClicked);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - PortraitWidget is null.")); }

	CreateAIController();
}

void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDrafted)
	{
		MoveToDestination(Destination);
	}
	else
	{
		if (CurrentTask)
		{
			MoveToDestination(TaskDestination);

			if (bHasReachedToTask)
			{
				FRotator Rotation = FMath::RInterpTo(GetActorRotation(), LookAtTaskRotation, DeltaTime, 5.0f);
				SetActorRotation(Rotation);
				if (!GetWorldTimerManager().IsTimerActive(FocusTaskTimer))
				{
					GetWorld()->GetTimerManager().SetTimer(FocusTaskTimer, this, &ASurvivor::StartDoingTask, 1.5f, false);
				}
			}
			else if (TaskState == ESurvivorTaskState::ESTS_Performing)
			{
				if (AnimInstance && !AnimInstance->Montage_IsPlaying(TaskMontage))
				{
					PlayTaskAnimation();
				}
			}
		}
		else
		{
			MoveToDestination(Destination);
		}
	}
}

void ASurvivor::SetCharacterSettings()
{
	bCanAffectNavigationGeneration = true;
}

void ASurvivor::GetReferences()
{
	GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		MyHUD = GameManager->GetMyHUD();
		if (!MyHUD) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::GetReferences - MyHUD is null.")); }

		PortraitWidget = GameManager->GetPortraitWidget();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::GetReferences - GameManager is null.")); }

	AnimInstance = Cast<USurvivorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ASurvivor::OnNotifyBegin);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::GetReferences - AnimInstance is null.")); }
}

/*
	COMPONENTS
*/
void ASurvivor::SetCapsuleComponent()
{
	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		CapsuleComponent->SetGenerateOverlapEvents(false);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block); // Block mouse
		CapsuleComponent->SetCanEverAffectNavigation(true);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetCapsuleComponent - CapsuleComponent is null.")); }
}

void ASurvivor::SetSkeletalMeshComponent()
{
	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetSkeletalMeshComponent - MeshComponent is null.")); }
}

void ASurvivor::SetCharacterMovement()
{
	CharMovement = GetCharacterMovement();
	if (CharMovement)
	{
		CharMovement->MaxWalkSpeed = 250.0f;
		bUseControllerRotationYaw = false;
		CharMovement->bOrientRotationToMovement = true;
		CharMovement->bUseRVOAvoidance = true;
		CharMovement->AvoidanceConsiderationRadius = 50.f;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetCharacterMovement - CharMovement is null")); }
}

void ASurvivor::OnPortraitClicked()
{
	if (MyHUD)
	{
		MyHUD->DeselectAll();
		MyHUD->Select(this);
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("ASurvivor::OnPortraitClicked - MyHUD is null")); }
}

void ASurvivor::CreateAIController()
{
	MyAIController = Cast<AMyAIController>(GetController()); // For some reason newly spawned survivors spawn without AI controllers
	if (!MyAIController)
	{
		MyAIController = GetWorld()->SpawnActor<AMyAIController>(AMyAIController::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
		if (MyAIController)
		{
			MyAIController->Possess(this);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - MyAIController is null")); }
	}
}

void ASurvivor::StartDoingTask()
{
	if (bHasReachedToTask) { bHasReachedToTask = false; }
	
	GetWorldTimerManager().ClearTimer(FocusTaskTimer);

	if (TaskState != ESurvivorTaskState::ESTS_Performing)
	{
		TaskState = ESurvivorTaskState::ESTS_Performing;
	}

	PlayTaskAnimation();
}

void ASurvivor::SetbCanMove(bool Value1)
{
	if (Value1)
	{
		if (!bCanMove) { bCanMove = true; }
	}
	else
	{
		if (bCanMove) { bCanMove = false; }
	}
}

void ASurvivor::SetTask(AHarvestable* Harvestable1)
{
	if (Harvestable1)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetSurroundDestination - Navigation system is null.")); return; }

		FVector CenterPoint = Harvestable1->GetActorLocation();
		if(Harvestable1->ActorHasTag("Stone")) { HarvestRadius = 120.0f; }
		else if (Harvestable1->ActorHasTag("Tree")) { HarvestRadius = 140.0f; }

		bool bFoundValidDestination{};
		FNavLocation ProjectedLocation{};
		FVector ClosestDestination;
		float MinDistance = FLT_MAX;

		if (PortraitWidget)
		{
			float AngleBetweenSurvivors = 360.0f / PortraitWidget->GetCurrentSurvivors().Num();

			for (int32 i = 0; i < PortraitWidget->GetCurrentSurvivors().Num(); i++)
			{
				float Angle = FMath::DegreesToRadians(AngleBetweenSurvivors * i);
				FVector Offset = FVector(FMath::Cos(Angle) * HarvestRadius, FMath::Sin(Angle) * HarvestRadius, 0.0f);
				FVector GoalDestination = CenterPoint + Offset;

				if (GameManager->GetReservedDestinations().Contains(GoalDestination))
				{
					continue;
				}

				FNavLocation TempProjectedLocation;
				if (NavSys->ProjectPointToNavigation(GoalDestination, TempProjectedLocation))
				{
					float Distance = FVector::Dist(GetActorLocation(), TempProjectedLocation.Location);
					if (Distance < MinDistance)
					{
						MinDistance = Distance;
						ProjectedLocation = TempProjectedLocation;
						bFoundValidDestination = true;
					}
				}
			}
		}
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::CalculateTaskDestination - PortraitWidget is null.")); }

		if (bFoundValidDestination)
		{
			TasksArray.AddUnique(Harvestable1);
			TaskDestinationsArray.AddUnique(ProjectedLocation.Location);

			if (GameManager) { GameManager->AddToReservedDestinations(ProjectedLocation.Location); }
			else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::CalculateTaskDestination - GameManager is null.")); }

			LineUpTasks();

			if (TasksArray[0] == Harvestable1)
			{
				SetCurrentTask(TasksArray[0]);
				SetTaskDestination(TaskDestinationsArray[0]);
				
				if (!bIsDrafted)
				{
					SetbCanMove(true);
				}
			}
		}
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::CalculateTaskDestination - Couldn't find a unique destination for Harvestable1.")); }
	}
}

void ASurvivor::OnNotifyBegin(FName NotifyName1, const FBranchingPointNotifyPayload& BranchingPointPayload1)
{
	if (CurrentTask)
	{
		int32 HarvestDamage{ 10 };
		if (NotifyName1 == FName("HarvestHit"))
		{
			CurrentTask->ReduceHarvestHealth(HarvestDamage);
			if (CurrentTask->GetHarvestHealth() <= 0)
			{
				AActor* DestroyedActor{ CurrentTask };
				if (GameManager) { GameManager->RemoveFromTaskArrays(CurrentTask); }
				else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::OnNotifyBegin - GameManager is null.")); }

				CurrentTask->Destroy();

				if (PortraitWidget)
				{
					for (ASurvivor* Survivor : PortraitWidget->GetCurrentSurvivors())
					{
						if (Survivor->GetCurrentTask() == DestroyedActor)
						{
							Survivor->StopWorking();
							Survivor->LineUpTasks();
							Survivor->ResetPriorities();
						}
					}
				}
			}
		}
	}
}

void ASurvivor::LineUpTasks()
{
	if (TasksArray.Num() != TaskDestinationsArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ASurvivor::LineUpTasks - Task arrays and task destinations array are not the same length."));
		return;
	}

	TArray<int32> Indices{};
	Indices.SetNum(TasksArray.Num());
	for (int32 i = 0; i < Indices.Num(); i++)
	{
		Indices[i] = i;
	}

	Indices.Sort([this](int32 AIndex, int32 BIndex)
	{
		float DistanceA = FVector::Dist(GetActorLocation(), TasksArray[AIndex]->GetActorLocation());
		float DistanceB = FVector::Dist(GetActorLocation(), TasksArray[BIndex]->GetActorLocation());
		return DistanceA < DistanceB;
	});

	TArray<AHarvestable*> SortedTasksArray{};
	SortedTasksArray.SetNum(TasksArray.Num());
	TArray<FVector> SortedTaskDestinationsArray{};
	SortedTaskDestinationsArray.SetNum(TaskDestinationsArray.Num());

	for (int32 i = 0; i < Indices.Num(); ++i)
	{
		SortedTasksArray[i] = TasksArray[Indices[i]];
		SortedTaskDestinationsArray[i] = TaskDestinationsArray[Indices[i]];
	}

	TasksArray = SortedTasksArray;
	TaskDestinationsArray = SortedTaskDestinationsArray;
}

void ASurvivor::StopMovement(bool Value1)
{
	if (Value1)
	{
		if (MoveState != ESurvivorMoveState::ESMS_NONE) { MoveState = ESurvivorMoveState::ESMS_NONE; }
		if (!CapsuleComponent->CanEverAffectNavigation()) { CapsuleComponent->SetCanEverAffectNavigation(true); }
		if (MyAIController)
		{
			MyAIController->StopMovement();
		}
	}
	else
	{
		if (MoveState != ESurvivorMoveState::ESMS_Walking) { MoveState = ESurvivorMoveState::ESMS_Walking; }
		if (CapsuleComponent->CanEverAffectNavigation()) { CapsuleComponent->SetCanEverAffectNavigation(false); }
	}
}

void ASurvivor::StopWorking()
{
	if (WorkState != ESurvivorWorkState::ESWS_NONE) { WorkState = ESurvivorWorkState::ESWS_NONE; }
	if (GeneralState != ESurvivorGeneralState::ESGS_NONE) { GeneralState = ESurvivorGeneralState::ESGS_NONE; }
	if (TaskState != ESurvivorTaskState::ESTS_NONE) { TaskState = ESurvivorTaskState::ESTS_NONE; }
	if (TaskMontage && AnimInstance && AnimInstance->Montage_IsPlaying(TaskMontage))
	{
		float BlendOutRate{0.5f};
		AnimInstance->Montage_Stop(BlendOutRate, TaskMontage);
	}
	else if(!AnimInstance) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::StopWorking - AnimInstance is null.")); }
	else if(!TaskMontage) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::StopWorking - TaskMontage is null.")); }

	if (ToolInstance) { ToolInstance->Destroy(); ToolInstance = nullptr; }
}

void ASurvivor::MoveOnWithTimer()
{
	if (!GetWorldTimerManager().IsTimerActive(MoveOnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(MoveOnTimer, this, &ASurvivor::MoveOn, 0.5f, false);
	}
}

void ASurvivor::ResetPriorities()
{
	if (TasksArray.Num() > 0 && TaskDestinationsArray.Num() > 0)
	{
		SetCurrentTask(TasksArray[0]);
		SetTaskDestination(TaskDestinationsArray[0]);
		MoveOnWithTimer();
	}
	else
	{
		SetCurrentTask(nullptr);
		SetTaskDestination(FVector(0.0f, 0.0f, 0.0f));
		StopMovement(true);
	}
}

void ASurvivor::Equip(AActor* ToolInstance1, USceneComponent* InParent1, FName InSocketName1)
{
	UStaticMeshComponent* ToolMesh = ToolInstance1->FindComponentByClass<UStaticMeshComponent>();

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ToolMesh->AttachToComponent(InParent1, TransformRules, InSocketName1);
}

void ASurvivor::SetTool(TSubclassOf<AActor> Tool1, ESurvivorWorkState WorkState1)
{
	if (WorkState != WorkState1)
	{
		WorkState = WorkState1;
	}

	if (PickaxeClass)
	{
		const USkeletalMeshSocket* ToolSocket = MeshComponent->GetSocketByName(FName("ToolSocket"));

		FTransform SocketTransform{};
		if (ToolSocket) { SocketTransform = ToolSocket->GetSocketTransform(MeshComponent); }
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - ToolSocket is null")); }


		ToolInstance = GetWorld()->SpawnActor<AActor>(Tool1, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
		if (ToolInstance)
		{
			Equip(ToolInstance, MeshComponent, "ToolSocket");
		}
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - ToolInstance is null")); }
	}
}

void ASurvivor::PlayTaskAnimation()
{
	if (GeneralState == ESurvivorGeneralState::ESGS_Tasking)
	{
		AnimInstance->Montage_Play(TaskMontage);

		FName SectionName{};

		switch (TaskState)
		{
		case ESurvivorTaskState::ESTS_Preparing:
			switch (WorkState)
			{
			case ESurvivorWorkState::ESWS_MiningStone:
				SectionName = FName("PrepareMiningStone");
				break;
			case ESurvivorWorkState::ESWS_CuttingTree:
				SectionName = FName("PrepareCuttingTree");
				break;
			default:
				break;
			}
			break;
		case ESurvivorTaskState::ESTS_Performing:
			switch (WorkState)
			{
			case ESurvivorWorkState::ESWS_MiningStone:
				SectionName = FName("MiningStone");
				break;
			case ESurvivorWorkState::ESWS_CuttingTree:
				SectionName = FName("CuttingTree");
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, TaskMontage);
	}
}

void ASurvivor::SetbIsSelected(bool bIsSelected1)
{
	if (bIsSelected1)
	{
		if (!bIsSelected) { bIsSelected = true; }
	}
	else
	{
		if (bIsSelected) { bIsSelected = false; }
	}
}

void ASurvivor::SetbIsDrafted(bool bIsDrafted1)
{
	if (bIsDrafted1)
	{
		if (!bIsDrafted) { bIsDrafted = true; }
	}
	else
	{
		if (bIsDrafted) { bIsDrafted = false; }
	}

	if (bIsDrafted)
	{
		DraftedImage->SetVisibility(ESlateVisibility::Visible);
		SetDestination(FVector(0.0f, 0.0f, 0.0f));
		StopMovement(true);
		StopWorking();
		if (bHasReachedToTask) { bHasReachedToTask = false; }
	}
	else
	{
		SetDestination(FVector(0.0f, 0.0f, 0.0f));
		if (TasksArray.Num() > 1)
		{
			LineUpTasks();
			ResetPriorities();
		}

		DraftedImage->SetVisibility(ESlateVisibility::Hidden);
		if (CurrentTask)
		{
			SetbCanMove(true);
		}
	}
}

void ASurvivor::MoveToDestination(const FVector& Destination1)
{
	if (Destination1.IsNearlyZero(0) || !bCanMove) { return; }

	if (MyAIController)
	{
		MyAIController->MoveToLocation(Destination1, Acceptance, false, true);

		UPathFollowingComponent* PathFollowingComponent = MyAIController->GetPathFollowingComponent();
		if (!PathFollowingComponent) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - PathFollowingComponent is null.")); return; }

		if (PathFollowingComponent->DidMoveReachGoal())
		{
			StopMovement(true);

			if (Destination1 == TaskDestination && CurrentTask)
			{
				FVector LookAtDirection = CurrentTask->GetActorLocation() - GetActorLocation();
				LookAtTaskRotation = LookAtDirection.Rotation();

				if (!bHasReachedToTask) { bHasReachedToTask = true; }

				if (GeneralState != ESurvivorGeneralState::ESGS_Tasking) { GeneralState = ESurvivorGeneralState::ESGS_Tasking; }
				if (TaskState != ESurvivorTaskState::ESTS_Preparing) { TaskState = ESurvivorTaskState::ESTS_Preparing; }

				if (CurrentTask->ActorHasTag("Stone")) { SetTool(PickaxeClass, ESurvivorWorkState::ESWS_MiningStone); }
				else if (CurrentTask->ActorHasTag("Tree")) { SetTool(AxeClass, ESurvivorWorkState::ESWS_CuttingTree); }

				PlayTaskAnimation();

				SetbCanMove(false);
			}
		}
		else { StopMovement(false); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - MyAIController is null")); }
}
