#pragma once

#include "CoreMinimal.h"
#include "Widgets/MyUserWidget.h"
#include "HarvestWidget.generated.h"

class UButton;
class UTextBlock;

class AMyGameManager;
class AMyView;
class AMyHUD;
class UPortraitWidget* PortraitWidget{};

UCLASS()
class MORETIMETODIE_API UHarvestWidget : public UMyUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;

	/*
		REFERENCES
	*/
	AMyGameManager* GameManager{};
	AMyView* MyView{};
	AMyHUD* MyHUD{};
	UPortraitWidget* PortraitWidget{};

	UPROPERTY(meta = (BindWidget))
	UButton* HarvestButton{};

	UPROPERTY(meta = (BindWidget))
	UButton* StopHarvestingButton{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HarvestText{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StopHarvestingText{};

	UFUNCTION()
	void OnHarvestButton();

	UFUNCTION()
	void OnStopHarvestButton();

public:

	void SetButtonText(FString HarvestText1, FString StopHarvestingText1);
};
