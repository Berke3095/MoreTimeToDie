#include "MyHUD.h"

#include "MyGameManager.h"
#include "MyView.h"
#include "Characters/Survivor.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	AMyGameManager* GameManager = AMyGameManager::GetInstance(); // GameManager
	if (GameManager)
	{
		MyView = GameManager->GetMyView(); // MyView
		if (!MyView)
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyHUD::BeginPlay - MyView is null."));
		}
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
	else if (!MyView)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyHUD::DrawHUD - MyView is null."));
	}

	UE_LOG(LogTemp, Warning, TEXT("Selected Actors: %d"), SelectedActors.Num());
	UE_LOG(LogTemp, Warning, TEXT("Selected Survivors: %d"), SelectedSurvivors.Num());
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

		for (AActor* Actor : SelectedActorsCopy)
		{
			if (!TempSelectedActors.Contains(Actor))
			{
				Deselect(Actor);
			}
		}
	}
}

void AMyHUD::Select(AActor* Actor1)
{
	if (Actor1->ActorHasTag("Selectable"))
	{
		SelectedActors.AddUnique(Actor1);

		if (Actor1->IsA<ASurvivor>())
		{
			ASurvivor* Survivor = Cast<ASurvivor>(Actor1);
			if (Survivor)
			{
				SelectedSurvivors.AddUnique(Survivor);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Select - Survivor is null.")); }
		}
	}
}

void AMyHUD::Deselect(AActor* Actor1)
{
	if (Actor1->ActorHasTag("Selectable"))
	{
		SelectedActors.Remove(Actor1);

		if (Actor1->IsA<ASurvivor>())
		{
			ASurvivor* Survivor = Cast<ASurvivor>(Actor1);
			if (Survivor)
			{
				SelectedSurvivors.Remove(Survivor);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyHUD::Deselect - Survivor is null.")); }
		}
	}
}

void AMyHUD::DeselectAll()
{
	SelectedActors.Empty();
	SelectedSurvivors.Empty();
}
