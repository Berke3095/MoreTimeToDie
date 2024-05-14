#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyView.generated.h"

struct FInputActionValue;

class UInputMappingContext;
class UInputAction;

class AMyPlayerController;
class AMyHUD;

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MORETIMETODIE_API AMyView : public APawn
{
	GENERATED_BODY()

public:

	AMyView();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/*
		REFERENCES
	*/
	AMyPlayerController* PlayerController{};
	AMyHUD* MyHUD{};

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* ViewMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Move camera
	UInputAction* MoveCameraAction{};
	void MoveCamera(const FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Input") // MidMouse
	UInputAction* MidMouseAction{};
	void MidMouseStart();
	void MidMouseEnd();
	bool bMidMouseHeld{};

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Rotate camera
	UInputAction* RotateCameraAction{};
	void RotateCamera(const FInputActionValue& InputValue1);
	float MouseX{};
	float MouseY{};

	UPROPERTY(EditDefaultsOnly, Category = "Input") // Zoom camera
	UInputAction* ZoomCameraAction{};
	void ZoomCamera(const FInputActionValue& InputValue1);
	float CameraZoomSpeed{ 500.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LeftClickAction{};
	void LeftClickStart();
	void LeftClickTrigger();
	void LeftClickEnd();

	/*
		VIEW
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // SpringArm
	USpringArmComponent* SpringArm{};
	void SetupSpringArm();
	const float MinTargetArmLength{ 400.0f };
	const float MaxTargetArmLength{ 2500.0f };
	float CurrentTargetArmLength{MaxTargetArmLength};
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // Camera
	UCameraComponent* Camera{};
	void SetupCamera();
	const float CameraSpeed{ 500.0f };

	/*
		HUD
	*/
	FVector2D StartingRectanglePosition{};
	FVector2D EndingRectanglePosition{};
	bool bCanDraw{};

public:

	FORCEINLINE const bool GetbMidMouseHeld() const { return bMidMouseHeld; }

	FORCEINLINE const FVector2D& GetStartPointOfRec() const { return StartingRectanglePosition; }
	FORCEINLINE const FVector2D& GetEndingPointOfRec() const { return EndingRectanglePosition; }
	FORCEINLINE const bool GetbCanDraw() const { return bCanDraw; }
};
