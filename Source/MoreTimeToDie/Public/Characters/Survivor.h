#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

UCLASS()
class MORETIMETODIE_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:

	ASurvivor();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		COMPONENTS
	*/
	UCapsuleComponent* CapsuleComponent{};
	void SetCapsuleComponent();

	USkeletalMeshComponent* MeshComponent{};
	void SetSkeletalMeshComponent();

	UCharacterMovementComponent* CharMovement{};
	void SetCharacterMovement();

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* AvoidanceSphere{};
	void SetAvoidanceSphere();

};
