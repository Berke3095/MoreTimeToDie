#include "Characters/Survivor.h"

#include "MyGameManager.h" 

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Button.h"
#include "Components/Image.h" 

#include "MyAIController.h"
#include "Navigation/PathFollowingComponent.h"

#include "Widgets/PortraitWidget.h"
#include "MyHUD.h"
#include "Harvestables/Harvestable.h"

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
		if (GameManager && GameManager->GetAllTasks().Num() > 0)
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
		}
		else if (!GameManager) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::Tick - GameManager is null.")); }
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
	bHasReachedToTask = false;
	GetWorldTimerManager().ClearTimer(FocusTaskTimer);

	if (TaskState != ESurvivorTaskState::ESTS_Performing)
	{
		TaskState = ESurvivorTaskState::ESTS_Performing;
	}
}

void ASurvivor::SetbIsDrafted(bool bIsDrafted1)
{
	bIsDrafted = bIsDrafted1;

	if (bIsDrafted)
	{
		DraftedImage->SetVisibility(ESlateVisibility::Visible);
		SetDestination(FVector(0.0f, 0.0f, 0.0f));
		if (MyAIController) { MyAIController->StopMovement(); }

		if (WorkState != ESurvivorWorkState::ESWS_NONE)
		{
			WorkState = ESurvivorWorkState::ESWS_NONE;
		}
		if (GeneralState != ESurvivorGeneralState::ESGS_NONE)
		{
			GeneralState = ESurvivorGeneralState::ESGS_NONE;
		}
	}
	else
	{
		if (GameManager && GameManager->GetAllTasks().Num() > 0)
		{
			DraftedImage->SetVisibility(ESlateVisibility::Hidden);
			bCanMove = true;
		}
	}
}

void ASurvivor::MoveToDestination(const FVector& Destination1)
{
	if (Destination1.IsNearlyZero(0) || !bCanMove) { return; }

	if (MyAIController)
	{
		MyAIController->MoveToLocation(Destination1, Acceptance, false, true);

		if (GameManager && MyAIController->GetPathFollowingComponent()->DidMoveReachGoal() && MoveState != ESurvivorMoveState::ESMS_NONE)
		{
			if (MoveState != ESurvivorMoveState::ESMS_NONE) { MoveState = ESurvivorMoveState::ESMS_NONE; }
			if (!CapsuleComponent->CanEverAffectNavigation()) { CapsuleComponent->SetCanEverAffectNavigation(true); }
			bCanMove = false;

			if (Destination1 == TaskDestination && GameManager->GetAllTasks().Num() > 0)
			{
				FVector LookAtDirection = GameManager->GetAllTasks()[0]->GetActorLocation() - GetActorLocation();
				LookAtTaskRotation = LookAtDirection.Rotation();

				bHasReachedToTask = true;
				if (GeneralState != ESurvivorGeneralState::ESGS_Tasking)
				{
					GeneralState = ESurvivorGeneralState::ESGS_Tasking;
				}
				if (TaskState != ESurvivorTaskState::ESTS_Preparing)
				{
					TaskState = ESurvivorTaskState::ESTS_Preparing;
				}

				if (GameManager->GetAllTasks()[0]->ActorHasTag("Stone"))
				{
					if (WorkState != ESurvivorWorkState::ESWS_Mining)
					{
						WorkState = ESurvivorWorkState::ESWS_Mining;
					}
				}	
			}
		}
		else if(!GameManager) { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - GameManager is null.")); }
		else
		{
			if (MoveState != ESurvivorMoveState::ESMS_Walking) { MoveState = ESurvivorMoveState::ESMS_Walking; }
			if (CapsuleComponent->CanEverAffectNavigation()) { CapsuleComponent->SetCanEverAffectNavigation(false); }

			if (WorkState != ESurvivorWorkState::ESWS_NONE)
			{
				WorkState = ESurvivorWorkState::ESWS_NONE;
			}
			if (GeneralState != ESurvivorGeneralState::ESGS_NONE)
			{
				GeneralState = ESurvivorGeneralState::ESGS_NONE;
			}
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveToDestination - MyAIController is null")); }
}
