// Copyright W.V. Koren. All Rights Reserved


#include "UI/MyRadialSlider.h"

void UMyRadialSlider::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	// This ensures the visual is updated when SetValue is called
	SetValue(Value);
}

void UMyRadialSlider::SetOxygenValue(float InValue)
{
	Value =FMath::Clamp(InValue,0.f,1.f);
	SynchronizeProperties();
}
