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
	if (HUDWidgetRef)
	{
		HUDWidgetRef->UpdateShellCount(Value);
	}
}

void AInGameHUD::SetVisibility(bool bVisibility)
{
	if (HUDWidgetRef)
	{
		HUDWidgetRef -> SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}


