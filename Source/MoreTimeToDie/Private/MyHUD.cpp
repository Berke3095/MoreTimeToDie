#include "MyHUD.h"

#include "MyGameManager.h"
#include "MyView.h"

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
}

/*
	DRAW
*/
void AMyHUD::DrawBoxSelect(const FVector2D& StartingMousePosition1, FVector2D EndingMousePosition1)
{
	float Distance = FVector2D::Distance(StartingMousePosition1, EndingMousePosition1);
	if (Distance >= 10.0f) // Start drawing only when dragged
	{
		bIsDrawing = true;
		DrawLine(StartingMousePosition1.X, StartingMousePosition1.Y, EndingMousePosition1.X, StartingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(StartingMousePosition1.X, StartingMousePosition1.Y, StartingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(StartingMousePosition1.X, EndingMousePosition1.Y, EndingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);
		DrawLine(EndingMousePosition1.X, StartingMousePosition1.Y, EndingMousePosition1.X, EndingMousePosition1.Y, FLinearColor::White, RectangleThickness);
	}
}
