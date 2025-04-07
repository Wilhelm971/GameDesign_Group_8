// Copyright W.V. Koren. All Rights Reserved


#include "UI/GameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UGameHUDWidget::UGameHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameHUDWidget::UpdateShellCount(int32 Value)
{
	if (TXTShell)
	{
		TXTShell->SetText(FText::FromString(FString::FromInt(Value) + "Shells collected"));
	}
}

void UGameHUDWidget::UpdateOxygen(float Current, float Max)
{
	if (OxygenBar)
	{
		OxygenBar->SetPercent(Current/Max);
	}
}

void UGameHUDWidget::SetHUDVisibility(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
