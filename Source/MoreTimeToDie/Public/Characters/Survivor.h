#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnums/SurvivorEnums.h"
#include "Survivor.generated.h"

class UPortraitWidget;
class AMyHUD;
class AMyAIController;

class UButton;
class UImage;

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
	AMyAIController* MyAIController{};
	const float Acceptance{ 25.0f };
	FVector Destination{};
	bool bMoveOrdered{};
	/*
		STATES
	*/
	ESurvivorMoveState MoveState = ESurvivorMoveState::ESMS_NONE;

public:

	void SetbIsSelected(bool bIsSelected1) { bIsSelected = bIsSelected1; }
	FORCEINLINE const bool GetbIsSelected() const { return bIsSelected; }

	void SetbIsDrafted(bool bIsDrafted1);
	FORCEINLINE const bool GetbIsDrafted() const { return bIsDrafted; }
	void SetDraftedImage(UImage* DraftedImage1) { DraftedImage = DraftedImage1; }
	
	FORCEINLINE UButton* GetPortraitButton() const { return PortraitButton ? PortraitButton : nullptr; }
	void SetPortraitButton(UButton* PortraitButton1) { PortraitButton = PortraitButton1; }

	void SetDestination(FVector& Destination1) { Destination = Destination1; }
	FORCEINLINE const FVector& GetDestination() const { return Destination; }

	void MoveToDestination(FVector Destination1);

	FORCEINLINE const ESurvivorMoveState GetSurvivorMoveState() const { return MoveState; }
	void SetSurvivorMoveState(ESurvivorMoveState MoveState1) { MoveState = MoveState1; }

	void SetbMoveOrdered(bool Value1) { bMoveOrdered = Value1; }
};
