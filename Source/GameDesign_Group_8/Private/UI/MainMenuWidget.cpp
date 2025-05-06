// Copyright W.V. Koren. All Rights Reserved


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	// Runs a function when a button is clicked on
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
	
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
	}
	
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
	
	return true;
}

void UMainMenuWidget::OnStartClicked()
{
	UGameplayStatics::OpenLevel(this, "IntroCutsceneLevel");
	// Open Cutscene level

	
}

void UMainMenuWidget::OnSettingsClicked()
{
	UE_LOG(LogTemp, Display, TEXT("OnSettingsClicked"));

	if ( SettingsWidgetClass)
	{
		UUserWidget* SettingsWidget = CreateWidget<UUserWidget> (GetWorld(), SettingsWidgetClass);
		if (SettingsWidget)
		{
			// Create a setting widget and adds it to the screen
			SettingsWidget -> AddToViewport();
		}
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	// quits the game
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
