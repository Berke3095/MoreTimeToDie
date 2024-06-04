#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnums/SurvivorEnums.h"
#include "Survivor.generated.h"

class AMyGameManager;
class UPortraitWidget;
class AMyHUD;
class AMyAIController;

class UButton;
class UImage;

class USurvivorAnimInstance;

UCLASS()
class MORETIMETODIE_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:

	ASurvivor();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetCharacterSettings();
	void GetReferences();

	UPROPERTY(EditDefaultsOnly, Category = "Character HUD")
	UTexture* Portrait{};
	UPROPERTY(EditDefaultsOnly, Category = "Character HUD")
	FString Name{};

	/*
		COMPONENTS
	*/
	UCapsuleComponent* CapsuleComponent{};
	void SetCapsuleComponent();

	USkeletalMeshComponent* MeshComponent{};
	void SetSkeletalMeshComponent();

	UCharacterMovementComponent* CharMovement{};
	void SetCharacterMovement();

	/*
		REFERENCES
	*/
	AMyGameManager* GameManager{};
	UPortraitWidget* PortraitWidget{};
	AMyHUD* MyHUD{};

	/*
		WIDGETS
	*/
	UButton* PortraitButton{};
	UImage* DraftedImage{};
	UFUNCTION()
	void OnPortraitClicked();
	bool bIsSelected{};
	bool bIsDrafted{};

	/*
		MOVEMENT
	*/
	void CreateAIController();
	AMyAIController* MyAIController{};
	const float Acceptance{ -1.0f };
	FVector Destination{};

	void MoveToDestination(const FVector& Destination1);

	bool bCanMove{};

	/*
		STATES
	*/
	ESurvivorMoveState MoveState = ESurvivorMoveState::ESMS_NONE;
	ESurvivorWorkState WorkState = ESurvivorWorkState::ESWS_NONE;
	ESurvivorGeneralState GeneralState = ESurvivorGeneralState::ESGS_NONE;

	/*
		TASKS
	*/
	FVector TaskDestination{};
	FRotator LookAtTaskRotation{};
	bool bHasReachedToTask{};

	void StartDoingTask();
	FTimerHandle FocusTaskTimer{};

public:

	void SetbIsSelected(bool bIsSelected1) { bIsSelected = bIsSelected1; }
	FORCEINLINE const bool GetbIsSelected() const { return bIsSelected; }

	void SetbIsDrafted(bool bIsDrafted1);
	FORCEINLINE const bool GetbIsDrafted() const { return bIsDrafted; }
	void SetDraftedImage(UImage* DraftedImage1) { DraftedImage = DraftedImage1; }
	
	FORCEINLINE UButton* GetPortraitButton() const { return PortraitButton ? PortraitButton : nullptr; }
	void SetPortraitButton(UButton* PortraitButton1) { PortraitButton = PortraitButton1; }

	void SetDestination(const FVector& Destination1) { Destination = Destination1; }
	FORCEINLINE const FVector& GetDestination() const { return Destination; }

	FORCEINLINE const ESurvivorMoveState GetSurvivorMoveState() const { return MoveState; }
	FORCEINLINE const ESurvivorWorkState GetSurvivorWorkState() const { return WorkState; }
	FORCEINLINE const ESurvivorGeneralState GetSurvivorGeneralState() const { return GeneralState; }

	void SetTaskDestination(const FVector& TaskDestination1) { TaskDestination = TaskDestination1; }

	void SetbCanMove(bool Value1) { bCanMove = Value1; }
};
