#include "Widgets/PortraitWidget.h"

#include "MyGameManager.h"

#include "Components/Button.h" 
#include "Components/TextBlock.h" 

void UPortraitWidget::NativeConstruct()
{
	Super::NativeConstruct();

    GameManager = AMyGameManager::GetInstance();
    if (!GameManager){ UE_LOG(LogTemp, Warning, TEXT("UPortraitWidget::NativeConstruct - GameManager is null.")); }

    PortraitSlots[0] = Portrait0;
    PortraitSlots[1] = Portrait1;
    PortraitSlots[2] = Portrait2;
    PortraitSlots[3] = Portrait3;
    PortraitSlots[4] = Portrait4;
    PortraitSlots[5] = Portrait5;
    PortraitSlots[6] = Portrait6;

    NameSlots[0] = Name0;
    NameSlots[1] = Name1;
    NameSlots[2] = Name2;
    NameSlots[3] = Name3;
    NameSlots[4] = Name4;
    NameSlots[5] = Name5;
    NameSlots[6] = Name6;

    for (int32 i = 0; i < UE_ARRAY_COUNT(PortraitSlots); i++)
    {
        UButton* PortraitSlot = PortraitSlots[i];
        UTextBlock* NameSlot = NameSlots[i];
        if (PortraitSlot)
        {
            PortraitSlot->SetVisibility(ESlateVisibility::Hidden);
        }

        if (NameSlot)
        {
            NameSlot->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UPortraitWidget::SetSurvivorPortrait(UButton* PortraitSlot1, UTexture* PortraitImage1)
{
    if (PortraitSlot1 && PortraitImage1)
    {
        PortraitSlot1->WidgetStyle.Normal.SetResourceObject(PortraitImage1);
        PortraitSlot1->WidgetStyle.Hovered.SetResourceObject(PortraitImage1);
        PortraitSlot1->WidgetStyle.Pressed.SetResourceObject(PortraitImage1);
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UPortraitWidget::SetSurvivorPortrait - PortraitSlot or PortraitImage is null.")); }
}

void UPortraitWidget::SetSurvivorName(UTextBlock* NameSlot1, FString SurvivorName1)
{
    if (NameSlot1)
    {
        NameSlot1->SetText(FText::FromString(SurvivorName1));
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UPortraitWidget::SetSurvivorPortrait - NameSlot or SurvivorName is null.")); }
}

void UPortraitWidget::SetSurvivorHud(UTexture* PortraitImage1, FString SurvivorName1, ASurvivor* Survivor1)
{
    for (int32 i = 0; i < UE_ARRAY_COUNT(PortraitSlots); i++)
    {
        if (!PortraitSlots[i]->IsVisible() && !NameSlots[i]->IsVisible())
        {
            PortraitSlots[i]->SetVisibility(ESlateVisibility::Visible);
            NameSlots[i]->SetVisibility(ESlateVisibility::Visible);

            SetSurvivorPortrait(PortraitSlots[i], PortraitImage1);
            SetSurvivorName(NameSlots[i], SurvivorName1);

            CurrentSurvivors[i] = Survivor1;

            break;
        }
    }
}
