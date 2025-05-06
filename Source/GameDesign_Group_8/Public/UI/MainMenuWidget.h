// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	// Functions when buttons are clicked
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnQuitClicked();

protected:
	// Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	// Slot for Settings
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> SettingsWidgetClass;
	
	
};
