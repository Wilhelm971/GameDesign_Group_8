// Copyright W.V. Koren. All Rights Reserved


#include "UI/MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Adds the main menu widget to the screen
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();

			
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->SetShowMouseCursor(true); // Shows mouse
				PlayerController->SetInputMode(FInputModeUIOnly()); // Makes input mode UI
			}
		}


	}
}
