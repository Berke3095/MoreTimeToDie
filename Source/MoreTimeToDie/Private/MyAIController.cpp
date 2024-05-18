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

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        float Randomizer = FMath::RandRange(-50.0f, 50.0f);
        FVector OffsetVector = FormationOffsets[i];
        OffsetVector.X += Randomizer;
        OffsetVector.Y += Randomizer;
        FoundDestinations.AddUnique(CenterPoint + OffsetVector);
    }

    for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
    {
        MoveableSurvivors[i]->SetDestination(FoundDestinations[i]);
    }

    MoveableSurvivors.Empty();
    FoundDestinations.Empty();
}
