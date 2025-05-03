// Copyright W.V. Koren. All Rights Reserved


#include "UI/GameHUDWidget.h"
#include "Components/TextBlock.h"



UGameHUDWidget::UGameHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGameHUDWidget::UpdateShellCount(int32 Value)
{

}

void UGameHUDWidget::UpdateOxygen(float Current, float Max)
{

	if (OxygenRadial)
	{
		//float Percent = FMath::Clamp(Current / Max, 0.f, 1.f);
		//OxygenRadial->SetValue(Percent);
		UE_LOG(LogTemp, Warning, TEXT("OxygenBar updated: %f"), Current / Max);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OxygenRadial is NULL"));
	}
}

