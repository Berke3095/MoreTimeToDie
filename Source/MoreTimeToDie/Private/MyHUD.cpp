#include "MyHUD.h"

#include "MyGameManager.h"
#include "MyView.h"
#include "Characters/Survivor.h"
#include "Harvestables/Harvestable.h"

#include "Widgets/PortraitWidget.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AMyGameManager* GameManager = AMyGameManager::GetInstance(); // GameManager
	if (GameManager)
	{
		MyView = GameManager->GetMyView(); // MyView
		if (!MyView){ UE_LOG(LogTemp, Warning, TEXT("AMyHUD::BeginPlay - MyView is null.")); }

		PortraitWidget = GameManager->GetPortraitWidget();
		if(!PortraitWidget){ UE_LOG(LogTemp, Warning, TEXT("AMyHUD::BeginPlay - PortraitWidget is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::BeginPlay - GameManager is null.")); }
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (MyView && MyView->GetbCanDraw())
	{
		DrawBoxSelect(MyView->GetStartPointOfRec(), MyView->GetEndingPointOfRec());
	}
	else if (!MyView){ UE_LOG(LogTemp, Warning, TEXT("AMyHUD::DrawHUD - MyView is null.")); }

	/*UE_LOG(LogTemp, Warning, TEXT("Number of Selected Actors: %d"), SelectedActors.Num());*/
	// UE_LOG(LogTemp, Warning, TEXT("Number of Selected Survivors: %d"), SelectedSurvivors.Num());
}

/*
	DRAW
*/
void AMyHUD::DrawBoxSelect(const FVector2D& StartingMousePosition1, FVector2D EndingMousePosition1)
{
	float Distance = FVector2D::Distance(StartingMousePosition1, EndingMousePosition1);
	if (Distance >= 10.0f) // Start drawing only when dragged
	{
		// Draw
		DrawLine(StartingMousePosition1.X, StartingMousePosition1.Y, EndingMousePosition1.X, StartingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(StartingMousePosition1.X, StartingMousePosition1.Y, StartingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(StartingMousePosition1.X, EndingMousePosition1.Y, EndingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(EndingMousePosition1.X, StartingMousePosition1.Y, EndingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);

		// Capture
		TArray<AActor*> TempSelectedActors{}; // Resets every time
		TArray<AActor*> SelectedActorsCopy = SelectedActors; // Created a copy in order to avoid errors such as "Modifying while iterating"

		GetActorsInSelectionRectangle<AActor>(StartingMousePosition1, EndingMousePosition1, TempSelectedActors, false);
		for (AActor* Actor : TempSelectedActors)
		{
			Select(Actor);
		}

		if (MyView && !MyView->GetbCtrlHeld())
		{
			for (AActor* Actor : SelectedActorsCopy)
			{
				if (!TempSelectedActors.Contains(Actor))
				{
					Deselect(Actor);
				}
			}
		}
		/*UE_LOG(LogTemp, Warning, TEXT("Number of TempSelected Actors: %d"), TempSelectedActors.Num());*/
	}
}

void AMyHUD::Highlight(AActor* SelectedActor1, UMaterialInterface* SelectedOverlayMat1)
{
	if (SelectedActor1)
	{
		UStaticMeshComponent* StaticMeshComponent = SelectedActor1->FindComponentByClass<UStaticMeshComponent>();
		USkeletalMeshComponent* SkeletalMeshComponent = SelectedActor1->FindComponentByClass<USkeletalMeshComponent>();

		if (SkeletalMeshComponent){ SkeletalMeshComponent->SetOverlayMaterial(SelectedOverlayMat1); }
		else if (StaticMeshComponent) { StaticMeshComponent->SetOverlayMaterial(SelectedOverlayMat1); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Highlight - MeshComponent is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Highlight - Actor is null.")); }
}

void AMyHUD::Select(AActor* Actor1)
{
	if (MyView && MyView->GetbCtrlHeld()) { Deselect(Actor1); return; }

	if (Actor1->ActorHasTag("Selectable"))
	{
		SelectedActors.AddUnique(Actor1);

		if (Actor1->IsA<ASurvivor>())
		{
			ASurvivor* Survivor = Cast<ASurvivor>(Actor1);
			if (Survivor)
			{
				Survivor->SetbIsSelected(true);
				Highlight(Survivor, SurvivorOverlayMat);
				if (PortraitWidget)
				{
					PortraitWidget->SetTintAlpha(Survivor->GetPortraitButton(), 1.0f, 1.0f, 1.0f);
					if (!Survivor->GetbIsDrafted())
					{
						PortraitWidget->SetButtonVisibility(PortraitWidget->GetDraftButton(), true);
					}
					else
					{
						PortraitWidget->SetButtonVisibility(PortraitWidget->GetUnDraftButton(), true);
					}
				}
				else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Select - PortraitWidget is null.")); }

				SelectedSurvivors.AddUnique(Survivor);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Select - Survivor is null.")); }
		}
		else if (Actor1->IsA<AHarvestable>())
		{
			AHarvestable* Harvestable = Cast<AHarvestable>(Actor1);
			if (Harvestable)
			{
				Highlight(Harvestable, HarvestableOverlayMat);
				SelectedHarvestables.AddUnique(Harvestable);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Select - Harvestable is null.")); }
		}
	}
}

void AMyHUD::Deselect(AActor* Actor1)
{
	if (MyView && MyView->GetbShiftHeld()) { return; }
	else if (!MyView) { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Deselect - MyView is null.")); }

	if (Actor1->ActorHasTag("Selectable"))
	{
		Highlight(Actor1, nullptr);
		SelectedActors.Remove(Actor1);

		if (Actor1->IsA<ASurvivor>())
		{
			ASurvivor* Survivor = Cast<ASurvivor>(Actor1);
			if (Survivor)
			{
				Survivor->SetbIsSelected(false);
				SelectedSurvivors.Remove(Survivor);

				if (PortraitWidget)
				{
					PortraitWidget->SetTintAlpha(Survivor->GetPortraitButton(), PortraitWidget->AlphaNormal, PortraitWidget->AlphaHovered, PortraitWidget->AlphaPressed);
					if (SelectedSurvivors.Num() == 0)
					{
						PortraitWidget->SetButtonVisibility(PortraitWidget->GetDraftButton(), false);
						PortraitWidget->SetButtonVisibility(PortraitWidget->GetUnDraftButton(), false);
					}
				}
				else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Deselect - PortraitWidget is null.")); }
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Deselect - Survivor is null.")); }
		}
		else if (Actor1->IsA<AHarvestable>())
		{
			AHarvestable* Harvestable = Cast<AHarvestable>(Actor1);
			if (Harvestable)
			{
				SelectedHarvestables.Remove(Harvestable);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Deselect - Harvestable is null.")); }
		}
	}
}

void AMyHUD::DeselectAll()
{
	if (MyView && (MyView->GetbShiftHeld() || MyView->GetbCtrlHeld())) { return; }
	else if(!MyView){ UE_LOG(LogTemp, Warning, TEXT("AMyHUD::DeselectAll - MyView is null.")); }

	TArray<AActor*> SelectedActorsCopy = SelectedActors;

	for (AActor* Actor : SelectedActorsCopy)
	{
		Deselect(Actor);
	}
	SelectedActorsCopy.Empty();
}
