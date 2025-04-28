// Copyright W.V. Koren. All Rights Reserved


#include "TestLevelGamemode.h"
#include "Kismet/GameplayStatics.h"


void ATestLevelGamemode::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
