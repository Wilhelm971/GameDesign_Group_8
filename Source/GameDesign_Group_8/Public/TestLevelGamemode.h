// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestLevelGamemode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API ATestLevelGamemode : public AGameModeBase 
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
