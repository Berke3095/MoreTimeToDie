#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HarvestWidget.generated.h"

UCLASS()
class MORETIMETODIE_API UHarvestWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;
};
