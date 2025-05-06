// Copyright W.V. Koren. All Rights Reserved


#include "UI/InGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/GameHUDWidget.h"




void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();


	if (HUDWidgetClass)
	{
		HUDWidgetRef = CreateWidget<UGameHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidgetRef)
		{
			HUDWidgetRef-> AddToViewport();
		}
	}
	
	
}

void AInGameHUD::UpdateShellCount(int32 Value)
{

}

void AInGameHUD::SetVisibility(bool bVisibility)
{
	// Flip flop beteween widgets is visible or not
	if (HUDWidgetRef)
	{
		HUDWidgetRef -> SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}


