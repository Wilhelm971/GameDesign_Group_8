// Copyright W.V. Koren. All Rights Reserved



#include "UI/ShellWidget.h"

UShellWidget::UShellWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UShellWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShellWidget::UpdateShellCount(int32 Value)
{
	if (TXTShell)
	{
		TXTShell->SetText(FText::FromString(FString::FromInt(Value) + "Shells collected"));
	}
}
