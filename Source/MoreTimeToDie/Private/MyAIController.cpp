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
        FVector(125.0f, -125.0f, 0.0f),
        FVector(-125.0f, -125.0f, 0.0f),
        FVector(-125.0f, 125.0f, 0.0f),
        FVector(125.0f, 125.0f, 0.0f),
        FVector(200.0f, 0.0f, 0.0f),
        FVector(-200.0f, 0.0f, 0.0f)
    };

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyAIController::SetDestinations - Navigation system is null.")); }

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        float Randomizer = FMath::RandRange(-20.0f, 20.0f);
        FVector OffsetVector = FormationOffsets[i];
        OffsetVector.X += Randomizer;
        OffsetVector.Y += Randomizer;
        FVector Destination = CenterPoint + OffsetVector;

        FNavLocation ProjectedLocation{};
        if (NavSys && NavSys->ProjectPointToNavigation(Destination, ProjectedLocation))
        {
            FoundDestinations.AddUnique(ProjectedLocation.Location);
        }
        else
        {
            if (NavSys && NavSys->ProjectPointToNavigation(Destination, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
            {
                FoundDestinations.AddUnique(ProjectedLocation.Location);
            }
        }
    }

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        MoveableSurvivors[i]->SetDestination(FoundDestinations[i]);
    }

    MoveableSurvivors.Empty();
    FoundDestinations.Empty();
}
