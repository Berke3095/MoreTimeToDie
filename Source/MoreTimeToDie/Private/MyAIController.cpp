#include "MyAIController.h"

#include "NavigationSystem.h"

#include "MyGameManager.h"
#include "MyHUD.h"
#include "Characters/Survivor.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyAIController::SetDestinations(FVector& CenterPoint)
{
    TArray<ASurvivor*> MoveableSurvivors{};
    TArray <FVector> FoundDestinations{};

    AMyGameManager* GameManager = AMyGameManager::GetInstance();
    if (GameManager)
    {
        AMyHUD* MyHUD = GameManager->GetMyHUD();
        if (MyHUD)
        {
            for (ASurvivor* Survivor : MyHUD->GetSelectedSurvivors())
            {
                if (Survivor->GetbIsDrafted())
                {
                    MoveableSurvivors.AddUnique(Survivor);
                }
            }
        }
        else { UE_LOG(LogTemp, Warning, TEXT("AMyAIController::FindDestinations - MyHUD is null.")); }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("AMyAIController::FindDestinations - MyManager is null.")); }

    const TArray<FVector> FormationOffsets = {
        FVector(0.0f, 0.0f, 0.0f),
        FVector(100.0f, -100.0f, 0.0f),
        FVector(-100.0f, -100.0f, 0.0f),
        FVector(-100.0f, 100.0f, 0.0f),
        FVector(100.0f, 100.0f, 0.0f),
        FVector(150.0f, 0.0f, 0.0f),
        FVector(-150.0f, 0.0f, 0.0f)
    };

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyAIController::SetDestinations - Navigation system is null.")); }

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        FVector OffsetVector = FormationOffsets[i];
        FVector Destination = CenterPoint + OffsetVector;

        FNavLocation ProjectedLocation{};
        if (NavSys && NavSys->ProjectPointToNavigation(Destination, ProjectedLocation, FVector(100.0f, 100.0f, 100.0f)))
        {
            FoundDestinations.AddUnique(ProjectedLocation.Location);
        }
    }

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        if (MoveableSurvivors.IsValidIndex(i) && FoundDestinations.IsValidIndex(i))
        {
            MoveableSurvivors[i]->SetDestination(FoundDestinations[i]);
        }
        else { break; }
    }

    MoveableSurvivors.Empty();
    FoundDestinations.Empty();
}
