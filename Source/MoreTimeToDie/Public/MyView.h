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
		REFERENCES
	*/
	class AMyGameManager* GameManager{};
	class AMyPlayerController* PlayerController{};

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* ViewMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveCameraAction{};
	void MoveCamera(const struct FInputActionValue& InputValue1);

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

	const float CameraSpeed{ 500.0f };

};
