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

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Move camera
	class UInputAction* MoveCameraAction{};
	void MoveCamera(const struct FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Input") // MidMouse
	UInputAction* MidMouseAction{};
	void MidMouseStart();
	void MidMouseEnd();
	bool bMidMouseHeld{};

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Rotate camera
	class UInputAction* RotateCameraAction{};
	void RotateCamera(const struct FInputActionValue& InputValue1);
	float MouseX{};
	float MouseY{};

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Zoom camera
	class UInputAction* ZoomCameraAction{};
	void ZoomCamera(const struct FInputActionValue& InputValue1);
	float CameraZoomSpeed{ 500.0f };

	/*
		VIEW
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // SpringArm
	class USpringArmComponent* SpringArm{};
	void SetupSpringArm();
	const float MinTargetArmLength{ 400.0f };
	const float MaxTargetArmLength{ 2500.0f };
	float CurrentTargetArmLength{MaxTargetArmLength};
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // Camera
	class UCameraComponent* Camera{};
	void SetupCamera();
	const float CameraSpeed{ 500.0f };

public:

	const bool GetbMidMouseHeld() const { return bMidMouseHeld; }
};
