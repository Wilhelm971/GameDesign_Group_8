// Copyright W.V. Koren. All Rights Reserved


#include "UI/PauseScreenWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/OxygenBubble.h"
#include "UI/GameHUDWidget.h"
#include "UI/InGameHUD.h"


bool UPauseScreenWidget::Initialize()
{
	Super::Initialize();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnResumeClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UPauseScreenWidget::OnSettingsClicked);
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
	
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			if (PlayerController)
			{

				//Player->PauseGame();
				
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->bShowMouseCursor = false;
				UGameplayStatics::SetGamePaused(GetWorld(), false);
			}

			RemoveFromParent();


	AInGameHUD* GameHUDWidget = Cast<AInGameHUD>(PlayerController->GetHUD());
	if (GameHUDWidget)
	{
		GameHUDWidget->SetVisibility(true);
	}
	
	
		//if (HUDWidget)
		//{
		//	HUDWidget->SetHUDVisibility();
		//}
}

void UPauseScreenWidget::OnSettingsClicked()
{
	UE_LOG(LogTemp, Display, TEXT("SettingsClicked"));
}


void UPauseScreenWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, "LandscapeTest");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LandscapeTest");
}


void UPauseScreenWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
