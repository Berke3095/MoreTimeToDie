#include "MyGameManager.h"

#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyView.h"
#include "MyHUD.h"
#include "Widgets/PortraitWidget.h"

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
			if (!MyView){ UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - MyView is null.")); }

			MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if(!MyHUD){ UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - MyHUD is null.")); }
			
			PortraitWidget = CreateWidget<UPortraitWidget>(PlayerController, PortraitWidgetClass);
			if (PortraitWidget)
			{
				PortraitWidget->AddToViewport();
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - PortraitWidget is null.")); }
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - PlayerController is null.")); }
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - GameMode is null.")); }
}

void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

