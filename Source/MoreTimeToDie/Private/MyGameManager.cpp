#include "MyGameManager.h"

#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyView.h"
#include "MyHUD.h"
#include "Widgets/PortraitWidget.h"
#include "Widgets/HarvestWidget.h"

#include "Kismet/GameplayStatics.h"

AMyGameManager* AMyGameManager::Instance = nullptr;

AMyGameManager::AMyGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
	Instance = this;
	SetActorHiddenInGame(true);
}

void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetReferences();

	if (PortraitWidget)
	{
		PortraitWidget->AddToViewport();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::BeginPlay - PortraitWidget is null.")); }
}

void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGameManager::GetReferences()
{
	GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		PlayerController = Cast<AMyPlayerController>(GameMode->GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			MyView = Cast<AMyView>(PlayerController->GetPawn());
			if (!MyView) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - MyView is null.")); }

			MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if (!MyHUD) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - MyHUD is null.")); }

			PortraitWidget = CreateWidget<UPortraitWidget>(PlayerController, PortraitWidgetClass);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - PlayerController is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::GetReferences - GameMode is null.")); }
}

void AMyGameManager::CreateWidgetAtHarvest(AActor* Harvest1)
{
	if (HarvestWidgetClass)
	{
		if (HarvestWidget) { DestroyHarvestWidgets(); }

		HarvestWidget = CreateWidget<UHarvestWidget>(GetWorld(), HarvestWidgetClass);
		if (HarvestWidget)
		{
			HarvestWidget->AddToViewport();

			FVector2D ScreenPosition;
			if (PlayerController && UGameplayStatics::ProjectWorldToScreen(PlayerController, Harvest1->GetActorLocation(), ScreenPosition))
			{
				FVector2D ViewportSize{};
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				ScreenPosition.X = (ScreenPosition.X / ViewportSize.X) * ViewportSize.X;
				ScreenPosition.Y = (ScreenPosition.Y / ViewportSize.Y) * ViewportSize.Y;

				HarvestWidget->SetPositionInViewport(ScreenPosition);

				AActor* HoveredActor = PlayerController->GetHoveredActor();
				if (HoveredActor && HoveredActor->Tags.Contains("Stone"))
				{
					HarvestWidget->SetButtonText("Mine", "Stop Mining");
				}
			}
			else if (!PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - PlayerController is null.")); }
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - HarvestWidget is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyGameManager::CreateWidgetAtActor - HarvestWidgetClass is null.")); }
}

void AMyGameManager::DestroyHarvestWidgets()
{
	if (HarvestWidget)
	{
		HarvestWidget->RemoveFromViewport();
		HarvestWidget = nullptr;
	}
}
