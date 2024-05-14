#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameManager.generated.h"

class AMyGameMode;
class AMyPlayerController;
class AMyView;
class AMyHUD;
class UPortraitWidget;

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
	UPROPERTY(EditDefaultsOnly, Category = "HUD") // Portraits
	TSubclassOf<UPortraitWidget> PortraitWidgetClass{};
	UPortraitWidget* PortraitWidget{};

public:

	static AMyGameManager* GetInstance() { return Instance ? Instance : nullptr; }

	AMyGameMode* GetMyGameMode() const { return GameMode ? GameMode : nullptr; }
	AMyPlayerController* GetMyPlayerController() const { return PlayerController ? PlayerController : nullptr; }
	AMyView* GetMyView() const { return MyView ? MyView : nullptr; }
	AMyHUD* GetMyHUD() const { return MyHUD ? MyHUD : nullptr; }
	UPortraitWidget* GetPortraitWidget() const { return PortraitWidget ? PortraitWidget : nullptr; }
};

