// Copyright W.V. Koren. All Rights Reserved


#include "UI/InGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/GameHUDWidget.h"




void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UGameHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget-> AddToViewport();
		}
	}
	
}

void AInGameHUD::UpdateShellCount(int32 Value)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateShellCount(Value);
	}
}

void AInGameHUD::SetVisibility(bool bVisibility)
{
	if (HUDWidget)
	{
		HUDWidget -> SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}


