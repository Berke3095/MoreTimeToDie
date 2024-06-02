#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyView.generated.h"

struct FInputActionValue;

class UInputMappingContext;
class UInputAction;

class AMyGameManager;
class AMyPlayerController;
class AMyHUD;
class AHarvestable;

class USpringArmComponent;
class UCameraComponent;

const float MinTargetArmLength{ 400.0f };
const float MaxTargetArmLength{ 2500.0f };

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

	void GetReferences();

	/*
		REFERENCES
	*/
	AMyGameManager* GameManager{};
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShiftAction{};
	void ShiftStart();
	void ShiftEnd();
	bool bShiftHeld{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CtrlAction{};
	void CtrlStart();
	void CtrlEnd();
	bool bCtrlHeld{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RightClickAction{};
	void RightClickStart();
	void RightClickEnd();
	void SetDestinations(FVector& CenterPoint);
	FVector Destination{};
	void OrderMove();
	void HandleHarvestWidget();

	/*
		VIEW
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings") // SpringArm
	USpringArmComponent* SpringArm{};
	void SetupSpringArm();
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

	/*
		TASKS
	*/
	AHarvestable* HarvestableActor{};
	void SetSurroundDestinations(AHarvestable* Harvestable1);

public:

	FORCEINLINE const bool GetbMidMouseHeld() const { return bMidMouseHeld; }
	FORCEINLINE const bool GetbShiftHeld() const { return bShiftHeld; }
	FORCEINLINE const bool GetbCtrlHeld() const { return bCtrlHeld; }

	FORCEINLINE const FVector2D& GetStartPointOfRec() const { return StartingRectanglePosition; }
	FORCEINLINE const FVector2D& GetEndingPointOfRec() const { return EndingRectanglePosition; }
	FORCEINLINE const bool GetbCanDraw() const { return bCanDraw; }

	FORCEINLINE AHarvestable* GetHarvestable() const { return HarvestableActor ? HarvestableActor : nullptr; }
};
