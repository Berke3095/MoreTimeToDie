#include "MyPlayerController.h"

#include "MyGameManager.h"
#include "MyView.h"

AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		MyView = GameManager->GetMyView();
	}
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawTraceFromMouse(HitResult);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMyPlayerController::DrawTraceFromMouse(FHitResult& TraceHitResult1)
{
	if (GetWorld() && !MyView->GetbMidMouseHeld())
	{
		FVector WorldLocation{}, WorldDirection{};
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FVector Start = FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z);
		FVector End = Start + WorldDirection * MaxMouseRange;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult1,
			Start,
			End,
			ECollisionChannel::ECC_GameTraceChannel1 // MouseTrace
		);

		if (!TraceHitResult1.bBlockingHit)
		{
			TraceHitResult1.ImpactPoint = End;
		}
		else
		{
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult1.ImpactPoint,
				12.0f,
				12,
				FColor::Red
			);
		}
	}
}

