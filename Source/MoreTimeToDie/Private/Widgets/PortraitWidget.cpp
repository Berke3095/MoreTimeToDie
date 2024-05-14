#include "Widgets/PortraitWidget.h"

#include "MyGameManager.h"

void UPortraitWidget::NativeConstruct()
{
	Super::NativeConstruct();

    GameManager = AMyGameManager::GetInstance();
    if (!GameManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("UPortraitWidget::NativeConstruct - GameManager is null."));
    }

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
}
