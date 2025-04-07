// Copyright W.V. Koren. All Rights Reserved


#include "UI/PauseScreenWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/OxygenBubble.h"
#include "UI/GameHUDWidget.h"


bool UPauseScreenWidget::Initialize()
{
	Super::Initialize();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnResumeClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnMainMenuClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnQuitClicked);
	}

	return true;
	
}

void UPauseScreenWidget::OnResumeClicked()
{
	
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			if (PlayerController)
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->bShowMouseCursor = false;
				UGameplayStatics::SetGamePaused(GetWorld(), false);
			}

			RemoveFromParent();


	/*UGameHUDWidget* GameHUDWidget = Cast<UGameHUDWidget>();
	if (GameHUDWidget)
	{
		GameHUDWidget->SetHUDVisibility(true);
	}
	*/
	
		//if (HUDWidget)
		//{
		//	HUDWidget->SetHUDVisibility();
		//}
}

void UPauseScreenWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
}

void UPauseScreenWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
