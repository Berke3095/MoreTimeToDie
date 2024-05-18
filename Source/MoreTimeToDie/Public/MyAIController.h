#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class ASurvivor;

UCLASS()
class MORETIMETODIE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TArray<ASurvivor*> MoveableSurvivors{};
	TArray <FVector> FoundDestinations{};

public:

	void SetDestinations(FVector& CenterPoint);
};
