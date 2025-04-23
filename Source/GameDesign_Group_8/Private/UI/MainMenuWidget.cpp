// Copyright W.V. Koren. All Rights Reserved


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

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
	UGameplayStatics::OpenLevel(this, "UITesting");



	//TODO
	// Switch input mode to game

}

void UMainMenuWidget::OnSettingsClicked()
{
	UE_LOG(LogTemp, Display, TEXT("OnSettingsClicked"));
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
