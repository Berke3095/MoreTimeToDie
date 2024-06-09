#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Harvestable.generated.h"

UCLASS()
class MORETIMETODIE_API AHarvestable : public AActor
{
	GENERATED_BODY()
	
public:	

	AHarvestable();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* HarvestableMesh{};

	int32 HarvestHealth{ 200 };

	bool bReadyToBeHarvested{};

public:

	void ReduceHarvestHealth(int32 Damage1) { HarvestHealth -= Damage1; }
	FORCEINLINE const int32 GetHarvestHealth() const { return HarvestHealth; }

	void SetbReadyToBeHarvested(bool Value1);
	FORCEINLINE const bool GetbReadyToBeHarvested() const { return bReadyToBeHarvested; }
};
