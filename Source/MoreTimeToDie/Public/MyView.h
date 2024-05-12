#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyView.generated.h"

UCLASS()
class MORETIMETODIE_API AMyView : public APawn
{
	GENERATED_BODY()

public:

	AMyView();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
		VIEW
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // SpringArm
	class USpringArmComponent* SpringArm{};
	void SetupSpringArm();

	const float MaxTargetArmLength{ 2500.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // Camera
	class UCameraComponent* Camera{};
	void SetupCamera();

};
