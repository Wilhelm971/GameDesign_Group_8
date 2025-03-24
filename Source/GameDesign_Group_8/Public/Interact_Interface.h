

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interact_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteract_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEDESIGN_GROUP_8_API IInteract_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnInteract(AActor* CausingActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void ToggleHighlight(bool bEnableHighlight);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void BeginOverlap(AActor* CausingActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void EndOverlap(AActor* CausingActor);
	
};
