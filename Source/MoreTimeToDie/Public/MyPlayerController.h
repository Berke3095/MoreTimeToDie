#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class MORETIMETODIE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AMyPlayerController();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	/*
		REFERENCES
	*/
	class AMyView* MyView{};

	/*
		MOUSE
	*/
	FHitResult HitResult{};
	void DrawTraceFromMouse(FHitResult& TraceHitResult1);
	const float MaxMouseRange{ 10000.0f };

	AActor* HoveredActor{};

public:

	FORCEINLINE AActor* GetHoveredActor() const { return HoveredActor ? HoveredActor : nullptr; }
};
