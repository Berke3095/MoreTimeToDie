#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HarvestWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class MORETIMETODIE_API UHarvestWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* HarvestButton{};

	UPROPERTY(meta = (BindWidget))
	UButton* StopHarvestingButton{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HarvestText{};

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StopHarvestingText{};

	void SetTintAlpha(UButton* Button1, float Normal1, float Hovered1, float Pressed1);

	UFUNCTION()
	void OnHarvestButton();

public:

	void SetButtonText(FString HarvestText1, FString StopHarvestingText1);
};
