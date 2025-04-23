// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/RadialSlider.h"
#include "MyRadialSlider.generated.h"

/**
 * 
 */

// I needed to create my own Radial Slider, since Unreal's one didn't update the HUD
UCLASS()
class GAMEDESIGN_GROUP_8_API UMyRadialSlider : public URadialSlider
{
	GENERATED_BODY()

	public:
	virtual void SynchronizeProperties() override;

	void SetOxygenValue(float InValue);
	
};
