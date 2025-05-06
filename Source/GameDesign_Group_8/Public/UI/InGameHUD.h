// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"
#include "GameHUDWidget.h"

#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

	public:
	
	virtual void BeginPlay() override;


	UFUNCTION()
	void UpdateShellCount(int32 Value);
	
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UGameHUDWidget* HUDWidgetRef; // This is the reference the pawn will use

	
	
	UFUNCTION()
	void SetVisibility(bool bVisibility);

};
