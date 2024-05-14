#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortraitWidget.generated.h"

class UButton;
class UTextBlock;

class AMyGameManager;

UCLASS()
class MORETIMETODIE_API UPortraitWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;

	// Portraits
	UButton* PortraitSlots[7]{};

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
	UTextBlock* NameSlots[7]{};

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
};
