#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UCLASS()
class MORETIMETODIE_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
private:

	virtual void BeginPlay() override;
	virtual void DrawHUD() override; // Override the DrawHUD function, only way to be able to draw

	/*
		DRAW
	*/
	void DrawBoxSelect(const FVector2D& StartingMousePosition1, FVector2D EndingMousePosition1);
	const float RectangleThickness{ 2.0f };

	/*
		REFERENCES
	*/
	class AMyView* MyView{};

public:

	bool bIsDrawing{};
};
