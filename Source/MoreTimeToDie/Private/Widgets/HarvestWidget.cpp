#include "Widgets/HarvestWidget.h"

#include "Components/Button.h" 
#include "Components/TextBlock.h" 

#include "MyGameManager.h"
#include "MyView.h"
#include "Harvestables/Harvestable.h"
#include "Widgets/PortraitWidget.h"
#include "Characters/Survivor.h"
#include "MyHUD.h"

void UHarvestWidget::NativeConstruct()
{
	Super::NativeConstruct();

    const float AlphaNormal = 0.7;
    const float AlphaHovered = 0.9;
    const float AlphaPressed = 1.0;

    SetTintAlpha(HarvestButton, AlphaNormal, AlphaHovered, AlphaPressed);
    SetTintAlpha(StopHarvestingButton, AlphaNormal, AlphaHovered, AlphaPressed);

    HarvestButton->OnClicked.AddDynamic(this, &UHarvestWidget::OnHarvestButton);
}

void UHarvestWidget::OnHarvestButton()
{
    AMyGameManager* GameManager = AMyGameManager::GetInstance();
    if (GameManager)
    {
        AMyView* MyView = GameManager->GetMyView();
        UPortraitWidget* PortraitWidget = GameManager->GetPortraitWidget();
        if (MyView && PortraitWidget)
        {
            if (MyView->GetHarvestable()->ActorHasTag("Stone")) { GameManager->AddToStoneTasks(MyView->GetHarvestable()); }
            else if(MyView->GetHarvestable()->ActorHasTag("Tree")) { GameManager->AddToTreeTasks(MyView->GetHarvestable()); }

            MyView->GetHarvestable()->SetbReadyToBeHarvested(true);

            AMyHUD* MyHUD = GameManager->GetMyHUD();
            if (MyHUD) { MyHUD->Highlight(MyView->GetHarvestable(), MyHUD->GetHarvestableOverlayMat()); }
            else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - MyHUD is null.")); }

            for (ASurvivor* Survivor : PortraitWidget->GetCurrentSurvivors())
            {
                Survivor->SetTask(MyView->GetHarvestable());
            }
        }
        else if(!MyView) { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - MyView is null.")); }
        else if(!PortraitWidget) { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - PortraitWidget is null.")); }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - GameManager is null.")); }
}

void UHarvestWidget::OnStopHarvestButton()
{

}

void UHarvestWidget::SetButtonText(FString HarvestText1, FString StopHarvestingText1)
{
    if (HarvestText)
    {
        HarvestText->SetText(FText::FromString(HarvestText1));
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::SetButtonText - HarvestText is null.")); }

    if (StopHarvestingText)
    {
        StopHarvestingText->SetText(FText::FromString(StopHarvestingText1));
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::SetButtonText - StopHarvestingText is null.")); }
}
