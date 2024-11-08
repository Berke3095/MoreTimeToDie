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

	AMyGameManager* GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		MyView = GameManager->GetMyView();
		if (!MyView) { UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::BeginPlay - MyView is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::BeginPlay - GameManager is null.")); }
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

/*
	MOUSE
*/
void AMyPlayerController::DrawTraceFromMouse(FHitResult& TraceHitResult1)
{
	if (GetWorld() && MyView && !MyView->GetbMidMouseHeld())
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
			HoveredActor = nullptr;
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

			AActor* HitActor = TraceHitResult1.GetActor();
			if (HitActor) { HoveredActor = HitActor; }
		}
	}
	else if(!MyView) { UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::DrawTraceFromMouse - MyView is null.")); }
}

