#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Harvestable.generated.h"

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

	int32 HarvestHealth{ 100 };

	bool bReadyToBeHarvested{};

public:

	FORCEINLINE const int32 GetCurrentResource() const { return CurrentResource; }
	void SetCurrentResource(int32 CurrentResource1) { CurrentResource = CurrentResource1; }

	void ReduceHarvestHealth(int32 Damage1) { HarvestHealth -= Damage1; }
	FORCEINLINE const int32 GetHarvestHealth() const { return HarvestHealth; }

	void SetbReadyToBeHarvested(bool Value1);
	FORCEINLINE const bool GetbReadyToBeHarvested() const { return bReadyToBeHarvested; }
};
