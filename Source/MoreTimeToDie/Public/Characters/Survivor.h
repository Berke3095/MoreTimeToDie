#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnums/SurvivorEnums.h"
#include "Survivor.generated.h"

class AMyGameManager;
class UPortraitWidget;
class AMyHUD;
class AMyAIController;
class AHarvestable;

class UButton;
class UImage;

class USurvivorAnimInstance;
class UAnimMontage;

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
	ESurvivorTaskState TaskState = ESurvivorTaskState::ESTS_NONE;

	/*
		TASKS
	*/
	AHarvestable* CurrentTask{};
	FVector TaskDestination{};

	TArray<AHarvestable*> TasksArray{};
	TArray<FVector> TaskDestinationsArray{};

	void CalculateTaskDestination(AHarvestable* Harvestable1);
	float HarvestRadius{};

	FRotator LookAtTaskRotation{};
	bool bHasReachedToTask{};

	void StartDoingTask();
	FTimerHandle FocusTaskTimer{};
	
	UPROPERTY(EditDefaultsOnly, Category = "Tasks")
	bool bCanMineStone{ true };
	bool bCanCutTree{ true };

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName1, const FBranchingPointNotifyPayload& BranchingPointPayload1);

	FTimerHandle MoveOnTimer{};
	void MoveOn() { SetbCanMove(true); GetWorldTimerManager().ClearTimer(MoveOnTimer); }

	/*
		TOOLS
	*/
	void Equip(AActor* ToolInstance1, USceneComponent* InParent1, FName InSocketName1);

	AActor* ToolInstance{};

	void SetTool(TSubclassOf<AActor> Tool1, ESurvivorWorkState WorkState1);

	UPROPERTY(EditDefaultsOnly, Category = "Tools")
	TSubclassOf<AActor> PickaxeClass{};

	UPROPERTY(EditDefaultsOnly, Category = "Tools")
	TSubclassOf<AActor> AxeClass{};

	/*
		ANIMATION
	*/
	USurvivorAnimInstance* AnimInstance{};

	void PlayTaskAnimation();

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* TaskMontage{};

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
	FORCEINLINE const ESurvivorTaskState GetSurvivorTaskState() const { return TaskState; }

	void SetbCanMove(bool Value1) { bCanMove = Value1; }

	void SetTask(AHarvestable* Harvestable1);

	FORCEINLINE const bool GetbCanMineStone() const { return bCanMineStone; }
	FORCEINLINE const bool GetbCanCutTree() const { return bCanCutTree; }

	FORCEINLINE AHarvestable* GetCurrentTask() const { return CurrentTask ? CurrentTask : nullptr; }
	void SetCurrentTask(AHarvestable* CurrentTask1) { CurrentTask = CurrentTask1; }
	FORCEINLINE const FVector& GetTaskDestination() const { return TaskDestination; }
	void SetTaskDestination(const FVector& TaskDestination1) { TaskDestination = TaskDestination1; }

	FORCEINLINE const TArray<AHarvestable*> GetTasksArray() { return TasksArray; }
	FORCEINLINE const TArray<FVector>& GetTaskDestinationsArray() { return TaskDestinationsArray; }

	void RemoveFromTasksArray(AHarvestable* Harvestable1) { TasksArray.Remove(Harvestable1); }
	void RemoveFromTaskDestinationsArray(const FVector& TaskDestination1) { TaskDestinationsArray.Remove(TaskDestination1); }

	void StopWorking();

	void MoveOnWithTimer();
};
