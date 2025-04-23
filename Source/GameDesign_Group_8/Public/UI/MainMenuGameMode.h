// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;


	UPROPERTY()
	class UUserWidget* MainMenuWidget;
};
