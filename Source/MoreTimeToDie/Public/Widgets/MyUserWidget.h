#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UButton;

UCLASS()
class MORETIMETODIE_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetTintAlpha(UButton* Button1, float Normal1, float Hovered1, float Pressed1);
	void SetButtonVisibility(UButton* Button1, bool Visibility1);
};
