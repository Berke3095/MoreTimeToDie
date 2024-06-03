#include "Widgets/HarvestWidget.h"

#include "Components/Button.h" 
#include "Components/TextBlock.h" 

#include "MyGameManager.h"
#include "MyView.h"
#include "Harvestables/Harvestable.h"

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

void UHarvestWidget::SetTintAlpha(UButton* Button1, float Normal1, float Hovered1, float Pressed1)
{
    if (Button1)
    {
        FSlateBrush& NormalBrush = Button1->WidgetStyle.Normal;
        FSlateBrush& HoveredBrush = Button1->WidgetStyle.Hovered;
        FSlateBrush& PressedBrush = Button1->WidgetStyle.Pressed;

        FLinearColor TintColorNormal = NormalBrush.TintColor.GetSpecifiedColor();
        FLinearColor TintColorHovered = HoveredBrush.TintColor.GetSpecifiedColor();
        FLinearColor TintColorPressed = PressedBrush.TintColor.GetSpecifiedColor();

        TintColorNormal.A = Normal1;
        TintColorHovered.A = Hovered1;
        TintColorPressed.A = Pressed1;

        NormalBrush.TintColor = TintColorNormal;
        HoveredBrush.TintColor = TintColorHovered;
        PressedBrush.TintColor = TintColorPressed;
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::SetTintAlpha - Button1 is null.")); }
}

void UHarvestWidget::OnHarvestButton()
{
    AMyGameManager* GameManager = AMyGameManager::GetInstance();
    if (GameManager)
    {
        AMyView* MyView = GameManager->GetMyView();
        if (MyView)
        {
            GameManager->SetSurroundDestinations(MyView->GetHarvestable());
            if (MyView->GetHarvestable()->ActorHasTag("Stone"))
            {
                GameManager->AddToStoneTasks(MyView->GetHarvestable());
            }
        }
        else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - MyView is null.")); }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UHarvestWidget::OnHarvestButton - GameManager is null.")); }
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
