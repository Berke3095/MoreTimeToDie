#include "MyAIController.h"

#include "NavigationSystem.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyAIController::MoveToDestination(const FVector& Destination1, float Acceptance1)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("AMyAIController::MoveToDestination - Controlled pawn is null"));
        return;
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("AMyAIController::MoveToDestination - Navigation system is null"));
        return;
    }

    MoveToLocation(Destination1, Acceptance1);
}
