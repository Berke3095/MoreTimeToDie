#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameManager.generated.h"

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
	class AMyGameMode* GameMode{};
	class AMyPlayerController* PlayerController{};
	class AMyView* MyView{};
	class AMyHUD* MyHUD{};

public:

	static AMyGameManager* GetInstance() { return Instance ? Instance : nullptr; }

	class AMyGameMode* GetMyGameMode() const { return GameMode ? GameMode : nullptr; }
	class AMyPlayerController* GetMyPlayerController() const { return PlayerController ? PlayerController : nullptr; }
	class AMyView* GetMyView() const { return MyView ? MyView : nullptr; }
	class AMyHUD* GetMyHUD() const { return MyHUD ? MyHUD : nullptr; }
};

