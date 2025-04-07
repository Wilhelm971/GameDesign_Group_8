// Copyright W.V. Koren. All Rights Reserved


#include "UI/InGameHUD.h"

AInGameHUD::AInGameHUD()
{
}



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

void AInGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHUD::UpdateShellCount(int32 Value)
{
	if (ShellWidget)
	{
		ShellWidget->UpdateShellCount(Value);
	}
}


