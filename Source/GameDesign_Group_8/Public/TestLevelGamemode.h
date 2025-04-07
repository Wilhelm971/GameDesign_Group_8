// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTestGameMode.h"
#include "TestLevelGamemode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API ATestLevelGamemode : public AFunctionalTestGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


};
