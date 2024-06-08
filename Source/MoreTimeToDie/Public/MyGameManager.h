#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/HarvestWidget.h"
#include "MyGameManager.generated.h"

class AMyGameMode;
class AMyPlayerController;
class AMyView;
class AMyHUD;
class UPortraitWidget;
class UHarvestWidget;
class AHarvestable;

UCLASS()
class MORETIMETODIE_API AMyGameManager : public AActor
{
	GENERATED_BODY()

public:

	AMyGameManager();

private:

	static AMyGameManager* Instance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void GetReferences();

	/*
		REFERENCES
	*/
	AMyGameMode* GameMode{};
	AMyPlayerController* PlayerController{};
	AMyView* MyView{};
	AMyHUD* MyHUD{};

	/*
		WIDGETS
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPortraitWidget> PortraitWidgetClass{};
	UPortraitWidget* PortraitWidget{};

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UHarvestWidget> HarvestWidgetClass{};
	UHarvestWidget* HarvestWidget{};

	/*
		TASKS
	*/
	TArray<AHarvestable*> AllTasks{};
	TArray<FVector> ReservedDestinations{};
	TArray<AHarvestable*> StoneTasks{};
	TArray<AHarvestable*> TreeTasks{};

public:

	static AMyGameManager* GetInstance() { return Instance ? Instance : nullptr; }

	AMyGameMode* GetMyGameMode() const { return GameMode ? GameMode : nullptr; }
	AMyPlayerController* GetMyPlayerController() const { return PlayerController ? PlayerController : nullptr; }
	AMyView* GetMyView() const { return MyView ? MyView : nullptr; }
	AMyHUD* GetMyHUD() const { return MyHUD ? MyHUD : nullptr; }
	UPortraitWidget* GetPortraitWidget() const { return PortraitWidget ? PortraitWidget : nullptr; }
	UHarvestWidget* GetHarvestWidget() const { return HarvestWidget ? HarvestWidget : nullptr; }

	void CreateWidgetAtHarvest(AActor* Harvest1);
	void DestroyHarvestWidgets();

	void AddToStoneTasks(AHarvestable* Stone1) { StoneTasks.AddUnique(Stone1); AllTasks.AddUnique(Stone1); }
	void AddToTreeTasks(AHarvestable* Tree1) { TreeTasks.AddUnique(Tree1); AllTasks.AddUnique(Tree1); }
	/*FORCEINLINE const TArray<AHarvestable*> GetStoneTasks() const { return StoneTasks; }
	FORCEINLINE const TArray<AHarvestable*> GetTreeTasks() const { return TreeTasks; }
	FORCEINLINE const TArray<AHarvestable*> GetAllTasks() const { return AllTasks; }*/

	void RemoveFromTaskArrays(AHarvestable* Harvestable1);

	void AddToReservedDestinations(const FVector& ReservedDestination1) { ReservedDestinations.AddUnique(ReservedDestination1); }
	void RemoveFromReservedDestinations(const FVector& ReservedDestination1);
	FORCEINLINE const TArray<FVector>& GetReservedDestinations() const { return ReservedDestinations; }

	void SetDestinations(const FVector& CenterPoint);
};

