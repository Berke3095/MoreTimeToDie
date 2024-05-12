#include "MyGameManager.h"

#include "MyGameMode.h"
#include "MyPlayerController.h"

AMyGameManager* AMyGameManager::Instance = nullptr;

AMyGameManager::AMyGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
	Instance = this;
}

void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode()); // GameMode reference
	if (GameMode)
	{
		PlayerController = Cast<AMyPlayerController>(GameMode->GetWorld()->GetFirstPlayerController()); // PlayerController reference
		if (PlayerController)
		{
			MyView = Cast<AMyView>(PlayerController->GetPawn()); // MyView reference
			if (!MyView)
			{
				UE_LOG(LogTemp, Error, TEXT("AMyGameManager::BeginPlay - MyView is null."));
			}
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyGameManager::BeginPlay - PlayerController is null.")); }
	}
	else{ UE_LOG(LogTemp, Error, TEXT("AMyGameManager::BeginPlay - GameMode is null.")); }
}

void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

