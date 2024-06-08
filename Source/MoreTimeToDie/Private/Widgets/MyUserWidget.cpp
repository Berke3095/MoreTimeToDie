#include "Widgets/MyUserWidget.h"

#include "Components/Button.h" 

void UMyUserWidget::SetTintAlpha(UButton* Button1, float Normal1, float Hovered1, float Pressed1)
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
    else { UE_LOG(LogTemp, Warning, TEXT("UMyUserWidget::SetTintAlpha - Button1 is null.")); }
}

void UMyUserWidget::SetButtonVisibility(UButton* Button1, bool Visibility1)
{
    if (Button1)
    {
        if (Visibility1 && !Button1->IsVisible())
        {
            Button1->SetVisibility(ESlateVisibility::Visible);
        }
        else if (!Visibility1 && Button1->IsVisible())
        {
            Button1->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UMyUserWidget::SetButtonVisibility - Button1 is null.")); }
}
