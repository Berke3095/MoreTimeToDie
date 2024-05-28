#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/HarvestWidget.h"
#include "Harvestable.generated.h"

class UWidgetComponent;

const int32 MaxResource{ 200 };

UCLASS()
class MORETIMETODIE_API AHarvestable : public AActor
{
	GENERATED_BODY()
	
public:	

	AHarvestable();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	int32 CurrentResource{ MaxResource };

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* HarvestableMesh{};

	UWidgetComponent* WidgetComponent{};

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UHarvestWidget> HarvestWidgetClass{};

public:

	FORCEINLINE const int32 GetCurrentResource() const { return CurrentResource; }
	void SetCurrentResource(int32 CurrentResource1) { CurrentResource = CurrentResource1; }

	void CreateWidgetComponent(TSubclassOf<UUserWidget> AssignedWidgetClass1);

	FORCEINLINE const TSubclassOf<UHarvestWidget> GetHarvestWidgetClass() const { return HarvestWidgetClass ? HarvestWidgetClass : nullptr; }
};
