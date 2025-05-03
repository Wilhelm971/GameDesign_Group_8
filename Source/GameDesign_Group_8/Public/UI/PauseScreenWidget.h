// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/GameHUDWidget.h"
#include "PauseScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API UPauseScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;




	// Functions for buttons
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnMainMenuClicked();

	UFUNCTION()
	void OnQuitClicked();




	// Buttons
	protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> SettingsWidgetClass;
	




	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;

};
