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
	PrimaryActorTick.bCanEverTick = true;
	Instance = this;
	SetActorHiddenInGame(true);
}

void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetReferences();

	if (PortraitWidget)
	{
		PortraitWidget->AddToViewport();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - PortraitWidget is null.")); }
}

void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*UE_LOG(LogTemp, Warning, TEXT("AllTasks length: %d"), AllTasks.Num());
	UE_LOG(LogTemp, Warning, TEXT("StoneTasks length: %d"), StoneTasks.Num());
	UE_LOG(LogTemp, Warning, TEXT("TreeTasks length: %d"), TreeTasks.Num());*/
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
}

void AMyGameManager::SetDestinations(const FVector& CenterPoint)
{
	TArray<ASurvivor*> MoveableSurvivors{};
	if (MyHUD)
	{
		for (ASurvivor* Survivor : MyHUD->GetSelectedSurvivors())
		{
			if (Survivor->GetbIsDrafted())
			{
				MoveableSurvivors.AddUnique(Survivor);
			}
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetDestinations - MyHUD is null.")); }

	const TArray<FVector> FormationOffsets = {
		FVector(0.0f, 0.0f, 0.0f),
		FVector(100.0f, -100.0f, 0.0f),
		FVector(-100.0f, -100.0f, 0.0f),
		FVector(-100.0f, 100.0f, 0.0f),
		FVector(100.0f, 100.0f, 0.0f),
		FVector(150.0f, 0.0f, 0.0f),
		FVector(-150.0f, 0.0f, 0.0f)
	};

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetDestinations - Navigation system is null.")); return; }

	TArray <FVector> FoundDestinations{};
	for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
	{
		FVector OffsetVector = FormationOffsets[i];
		FVector LeaderDestination = CenterPoint + OffsetVector;

		FNavLocation ProjectedLocation{};
		if (NavSys && NavSys->ProjectPointToNavigation(LeaderDestination, ProjectedLocation))
		{
			FoundDestinations.AddUnique(ProjectedLocation.Location);
		}
	}

	for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
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
		if (HarvestWidget) { DestroyHarvestWidgets(); }

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

void AMyGameManager::DestroyHarvestWidgets()
{
	if (HarvestWidget)
	{
		HarvestWidget->RemoveFromViewport();
		HarvestWidget = nullptr;
	}
}
