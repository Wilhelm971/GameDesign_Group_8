// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"
#include "ShellWidget.h"

#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

	public:
	AInGameHUD();

	virtual void DrawHUD() override;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateShellCount(int32 Value);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShellWidgetClass;
	
	private:
	UShellWidget* ShellWidget;
	
};
