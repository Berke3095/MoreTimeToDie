#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class AMyView;

class UPortraitWidget;

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
	TArray<AActor*> SelectedActors{};
	TArray<class ASurvivor*> SelectedSurvivors{};

	void Highlight(AActor* SelectedActor1, UMaterialInterface* SelectedOverlayMat1);

	UPROPERTY(EditDefaultsOnly, Category = "Highlight Overlays")
	UMaterialInterface* SurvivorOverlayMat{};

	/*
		REFERENCES
	*/
	AMyView* MyView{};
	
	/*
		WIDGETS
	*/
	UPortraitWidget* PortraitWidget{};

public:

	void Select(AActor* Actor1);
	void Deselect(AActor* Actor1);
	void DeselectAll();

	FORCEINLINE const TArray<ASurvivor*> GetSelectedSurvivors() const { return SelectedSurvivors; }
};
