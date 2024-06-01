#include "MyAIController.h"
//#include "Navigation/CrowdFollowingComponent.h"

//AMyAIController::AMyAIController(const FObjectInitializer& ObjectInitializer)
//    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
//{
//
//}

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    /*UCrowdFollowingComponent* CFComponent = FindComponentByClass<UCrowdFollowingComponent>();
    if (CFComponent)
    {
        
    }*/
}

void AMyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}