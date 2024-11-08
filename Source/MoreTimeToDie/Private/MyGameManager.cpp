#include "MyGameManager.h"

#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyView.h"
#include "MyHUD.h"
#include "Widgets/PortraitWidget.h"
#include "Widgets/HarvestWidget.h"
#include "Harvestables/Harvestable.h"
#include "Characters/Survivor.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

AMyGameManager* AMyGameManager::Instance = nullptr;

AMyGameManager::AMyGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
	Instance = this;
	SetActorHiddenInGame(true);
}

void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetReferences();

	if (PortraitWidget) { PortraitWidget->AddToViewport(); }
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - PortraitWidget is null.")); }
}

void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyGameManager::GetReferences()
{
	GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		PlayerController = Cast<AMyPlayerController>(GameMode->GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			MyView = Cast<AMyView>(PlayerController->GetPawn());
			if (!MyView) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - MyView is null.")); }

			MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if (!MyHUD) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - MyHUD is null.")); }

			PortraitWidget = CreateWidget<UPortraitWidget>(PlayerController, PortraitWidgetClass);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - PlayerController is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - GameMode is null.")); }
}

void AMyGameManager::RemoveFromTaskArrays(AHarvestable* Harvestable1)
{
	if (AllTasks.Contains(Harvestable1)) { AllTasks.Remove(Harvestable1); }

	if (StoneTasks.Contains(Harvestable1)) { StoneTasks.Remove(Harvestable1); }
	else if (TreeTasks.Contains(Harvestable1)) { TreeTasks.Remove(Harvestable1); }

	if (PortraitWidget)
	{
		TArray<FVector> RemovedDestinations{};

		for (ASurvivor* Survivor : PortraitWidget->GetCurrentSurvivors())
		{
			if (Survivor->GetTasksArray().Contains(Harvestable1))
			{
				int32 TaskIndex = Survivor->GetTasksArray().Find(Harvestable1);
				if (TaskIndex != INDEX_NONE) { RemovedDestinations.Add(Survivor->GetTaskDestinationsArray()[TaskIndex]); }
				Survivor->RemoveFromTasksArray(Harvestable1);
			}
		}
		for (const FVector& RemovedDestination : RemovedDestinations) { RemoveFromReservedDestinations(RemovedDestination); }
	}
}

void AMyGameManager::RemoveFromReservedDestinations(const FVector& ReservedDestination1)
{
	ReservedDestinations.Remove(ReservedDestination1);

	if (PortraitWidget)
	{
		for (ASurvivor* Survivor : PortraitWidget->GetCurrentSurvivors())
		{
			if (Survivor->GetTaskDestinationsArray().Contains(ReservedDestination1))
			{
				Survivor->RemoveFromTaskDestinationsArray(ReservedDestination1);
			}
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::RemoveFromReservedDestinations - PortraitWidget is null.")); }
}

void AMyGameManager::SetDestinations(const FVector& CenterPoint)
{
	TArray<ASurvivor*> MoveableSurvivors{};
	if (MyHUD)
	{
		for (ASurvivor* Survivor : MyHUD->GetSelectedSurvivors())
		{
			if (Survivor->GetbIsDrafted()) { MoveableSurvivors.AddUnique(Survivor); }
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetDestinations - MyHUD is null.")); }

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetDestinations - Navigation system is null.")); return; }

	TArray<FVector> FoundDestinations{};
	int32 NumSurvivors = MoveableSurvivors.Num();

	float Radius{ 80.0f };
	float AngleStep = { 360.0f / NumSurvivors };

	if (NumSurvivors > 4 || NumSurvivors == 1)
	{
		Radius = 130.0f;

		FNavLocation ProjectedLocation1{};
		if (NavSys->ProjectPointToNavigation(CenterPoint, ProjectedLocation1, FVector(100.0f, 100.0f, 100.0f)))
		{
			FoundDestinations.AddUnique(ProjectedLocation1.Location);
		}

		AngleStep = 360.0f / (NumSurvivors - 1);

		for (int32 i = 1; i < NumSurvivors; i++)
		{
			FVector Destination{};
			if (FoundDestinations.Num() > 0) { Destination = FoundDestinations[0]; }
			float Angle = FMath::DegreesToRadians(AngleStep * i);
			Destination.X += Radius * FMath::Cos(Angle);
			Destination.Y += Radius * FMath::Sin(Angle);

			FNavLocation ProjectedLocation2{};
			if (NavSys->ProjectPointToNavigation(Destination, ProjectedLocation2, FVector(100.0f, 100.0f, 100.0f)))
			{
				FoundDestinations.AddUnique(ProjectedLocation2.Location);
			}
		}
	}
	else if (NumSurvivors == 2)
	{
		FVector ActorLocation1 = MoveableSurvivors[0]->GetActorLocation();
		FVector ActorLocation2 = MoveableSurvivors[1]->GetActorLocation();
		FVector Midpoint = (ActorLocation1 + ActorLocation2) / 2.0f;

		FVector Direction = (CenterPoint - Midpoint).GetSafeNormal();
		FVector Perpendicular = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal();

		for (int32 i = 0; i < NumSurvivors; i++)
		{
			FVector Destination = CenterPoint;
			float Offset = (i - (NumSurvivors - 1) / 2.0f) * Radius;
			Destination += Perpendicular * Offset;

			FNavLocation ProjectedLocation{};
			if (NavSys->ProjectPointToNavigation(Destination, ProjectedLocation, FVector(100.0f, 100.0f, 100.0f)))
			{
				FoundDestinations.AddUnique(ProjectedLocation.Location);
			}
		}

		if (FoundDestinations.Num() == NumSurvivors)
		{
			float Distance1 = FVector::Dist(MoveableSurvivors[0]->GetActorLocation(), FoundDestinations[0]);
			float Distance2 = FVector::Dist(MoveableSurvivors[0]->GetActorLocation(), FoundDestinations[1]);

			if (Distance1 <= Distance2)
			{
				MoveableSurvivors[0]->SetDestination(FoundDestinations[0]);
				MoveableSurvivors[0]->SetbCanMove(true);

				MoveableSurvivors[1]->SetDestination(FoundDestinations[1]);
				MoveableSurvivors[1]->SetbCanMove(true);
			}
			else
			{
				MoveableSurvivors[0]->SetDestination(FoundDestinations[1]);
				MoveableSurvivors[0]->SetbCanMove(true);

				MoveableSurvivors[1]->SetDestination(FoundDestinations[0]);
				MoveableSurvivors[1]->SetbCanMove(true);
			}


			MoveableSurvivors.Empty();
			FoundDestinations.Empty();

			return;
		}
	}
	else
	{
		for (int32 i = 0; i < NumSurvivors; i++)
		{
			FVector Destination = CenterPoint;
			float Angle = FMath::DegreesToRadians(AngleStep * i);
			Destination.X += Radius * FMath::Cos(Angle);
			Destination.Y += Radius * FMath::Sin(Angle);

			FNavLocation ProjectedLocation2{};
			if (NavSys->ProjectPointToNavigation(Destination, ProjectedLocation2, FVector(100.0f, 100.0f, 100.0f)))
			{
				FoundDestinations.AddUnique(ProjectedLocation2.Location);
			}
		}
	}

	for (int32 i = 0; i < NumSurvivors; i++)
	{
		if (MoveableSurvivors.IsValidIndex(i) && FoundDestinations.IsValidIndex(i))
		{
			MoveableSurvivors[i]->SetDestination(FoundDestinations[i]);
			MoveableSurvivors[i]->SetbCanMove(true);
		}
		else { break; }
	}

	MoveableSurvivors.Empty();
	FoundDestinations.Empty();
}

void AMyGameManager::CreateWidgetAtHarvest(AActor* Harvest1)
{
	if (HarvestWidgetClass)
	{
		if (HarvestWidget) { DestroyHarvestWidget(); }

		HarvestWidget = CreateWidget<UHarvestWidget>(GetWorld(), HarvestWidgetClass);
		if (HarvestWidget)
		{
			HarvestWidget->AddToViewport();

			FVector2D ScreenPosition;
			if (PlayerController && UGameplayStatics::ProjectWorldToScreen(PlayerController, Harvest1->GetActorLocation(), ScreenPosition))
			{
				FVector2D ViewportSize{};
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				ScreenPosition.X = (ScreenPosition.X / ViewportSize.X) * ViewportSize.X;
				ScreenPosition.Y = (ScreenPosition.Y / ViewportSize.Y) * ViewportSize.Y;

				HarvestWidget->SetPositionInViewport(ScreenPosition);

				AActor* HoveredActor = PlayerController->GetHoveredActor();
				if (HoveredActor && HoveredActor->Tags.Contains("Stone")) { HarvestWidget->SetButtonText("Mine", "Stop Mining"); }
				else if (HoveredActor && HoveredActor->Tags.Contains("Tree")) { HarvestWidget->SetButtonText("Cut", "Stop Cutting"); }
			}
			else if (!PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - PlayerController is null.")); }
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - HarvestWidget is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - HarvestWidgetClass is null.")); }
}

void AMyGameManager::DestroyHarvestWidget()
{
	if (HarvestWidget)
	{
		HarvestWidget->RemoveFromViewport();
		HarvestWidget = nullptr;
	}
}
