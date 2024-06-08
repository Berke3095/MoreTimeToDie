#pragma once

#include "CoreMinimal.h"
#include "Widgets/MyUserWidget.h"
#include "HarvestWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class MORETIMETODIE_API UHarvestWidget : public UMyUserWidget
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

	UFUNCTION()
	void OnHarvestButton();

public:

	void SetButtonText(FString HarvestText1, FString StopHarvestingText1);
};
