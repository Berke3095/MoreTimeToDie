#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

class UPortraitWidget;
class AMyHUD;

class UButton;
class USphereComponent;

UCLASS()
class MORETIMETODIE_API ASurvivor : public ACharacter
{
	GENERATED_BODY()

public:

	ASurvivor();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AvoidanceSphere{};
	void SetAvoidanceSphere();

	/*
		REFERENCES
	*/
	UPortraitWidget* PortraitWidget{};
	AMyHUD* MyHUD{};

	UFUNCTION()
	void OnPortraitClicked();

public:

	bool bIsSelected{};
	UButton* PortraitButton{};
};
