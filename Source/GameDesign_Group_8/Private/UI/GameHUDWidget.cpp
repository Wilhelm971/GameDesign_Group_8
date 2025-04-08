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
	UProgressBar* OxygenProgressBar = Cast<UProgressBar>(OxygenBar);
	if (OxygenProgressBar)
	{
		float Percent = FMath::Clamp(Current / Max, 0.f, 1.f);
		OxygenProgressBar->SetPercent(Percent);
		//UE_LOG(LogTemp, Warning, TEXT("OxygenBar updated: %f"), Current / Max);
	}
}

