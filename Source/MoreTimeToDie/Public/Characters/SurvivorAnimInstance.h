#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SurvivorAnimInstance.generated.h"

class ASurvivor;
class UCharacterMovementComponent;

UCLASS()
class MORETIMETODIE_API USurvivorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	ASurvivor* Survivor{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* SurvivorMovement{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float SurvivorDirection{};
};
