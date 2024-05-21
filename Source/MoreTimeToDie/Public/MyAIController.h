#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class ASurvivor;

UCLASS()
class MORETIMETODIE_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AMyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	void SetDestinations(FVector& CenterPoint);
};
