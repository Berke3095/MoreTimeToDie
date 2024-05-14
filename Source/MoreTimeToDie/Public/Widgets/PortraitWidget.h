#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortraitWidget.generated.h"

class UButton;
class UTextBlock;

class AMyGameManager;
class ASurvivor;

const int32 MaxSurvivorCount{ 7 };

UCLASS()
class MORETIMETODIE_API UPortraitWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;

	

	// Portraits
	UButton* PortraitSlots[MaxSurvivorCount]{};

	UPROPERTY(meta = (BindWidget))
	UButton* Portrait0{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait1{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait2{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait3{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait4{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait5{};
	UPROPERTY(meta = (BindWidget))
	UButton* Portrait6{};

	// Names
	UTextBlock* NameSlots[MaxSurvivorCount]{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name0{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name1{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name2{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name3{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name4{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name5{};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name6{};

	/*
		REFERENCES
	*/
	AMyGameManager* GameManager{};
	ASurvivor* CurrentSurvivors[MaxSurvivorCount];

	void SetSurvivorPortrait(UButton* PortraitSlot1, UTexture* PortraitImage1);
	void SetSurvivorName(UTextBlock* NameSlot1, FString SurvivorName1);

public:

	void SetSurvivorHud(UTexture* PortraitImage1, FString SurvivorName1, ASurvivor* Survivor1);

	// Default alphas for portraits
	const float AlphaNormal = 0.5;
	const float AlphaHovered = 0.8;
	const float AlphaPressed = 1.0;
	void SetTintAlpha(UButton* PortraitButton1, float Normal1, float Hovered1, float Pressed1);
};
