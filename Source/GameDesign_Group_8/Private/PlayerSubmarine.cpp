// Copyright W.V. Koren. All Rights Reserved


#include "PlayerSubmarine.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MeshPaintVisualize.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "UI/InGameHUD.h"
#include "UI/PauseScreenWidget.h"
#include "UI/GameHUDWidget.h"


// Sets default values
APlayerSubmarine::APlayerSubmarine()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the meash, camera and collision
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box -> SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));

	Box -> SetCollisionProfileName(TEXT("Pawn"));

	SubmarineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubmarineMesh"));
	SubmarineMesh -> SetupAttachment(RootComponent);
	SubmarineMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = SubmarineMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(RootComponent);
	SpringArm -> bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;



	// Defining MovementSpeed & Oxygen

	MoveForce = 30000.0f;
	TorqueForce = 200000.0f;
	ElevateForce = 15000.0f;

	MaxOxygen = 100.0f;
	CurrentOxygen = 60.0f;
	OxygenDrainRate = 4.0f;


	
	bIsPaused = false;

}

// Called when the game starts or when spawned
void APlayerSubmarine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSubmarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		CurrentOxygen -= OxygenDrainRate * DeltaTime;
		CurrentOxygen = FMath::Clamp(CurrentOxygen, 0.0f, MaxOxygen);

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor:: Red, FString::Printf(TEXT("Oxygen: %.1f / %.1f"), CurrentOxygen, MaxOxygen));

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UGameHUDWidget>(PlayerController, HUDWidgetClass);
			if (HUDWidget)
			{
				if (UGameHUDWidget* GameHUD = Cast<UGameHUDWidget>(HUDWidget))
				{
					GameHUD->UpdateOxygen(CurrentOxygen, MaxOxygen);
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Check");
				}
			}
			
		}

		if (CurrentOxygen <= 0.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, "Oxygen Drained !");
			LostGame();
		}
	}

	//StraightenBoatLevel();

}

// Called to bind functionality to input
void APlayerSubmarine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find an Input Mapping Context!"))
		}

	}
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Move);
		Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Rotate);
		Input->BindAction(ElevateAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Elevate);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Look);
		
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::InteractWithObject);
		Input->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerSubmarine::TogglePauseMenu);

	}

	else
	{
		
			UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input Component!"));
	}

}



void APlayerSubmarine::Move_Implementation(const FInputActionValue& InputValue)
{
	FVector2d InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{

		FVector ForwardsForce = InputVector.Y * MoveForce * SubmarineMesh -> GetForwardVector();
		
		
		if (InputVector.Y < 0)
		{
			FVector BackwardsForce = ForwardsForce * 0.5f;
			SubmarineMesh->AddForce(BackwardsForce);
			
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Moving Backwards")));
		}

		else
		{
			SubmarineMesh->AddForce(ForwardsForce);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Moving Forwards")));
	
		}
	}
	
}

void APlayerSubmarine::Rotate_Implementation(const FInputActionValue& InputValue)
{

	FVector InputVector = InputValue.Get<FVector>();
	if (IsValid(Controller))
	{
		SubmarineMesh->AddTorqueInRadians(SubmarineMesh->GetUpVector() * TorqueForce * InputVector);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Rotating")));
	}
}

void APlayerSubmarine::Elevate_Implementation(const FInputActionValue& InputValue)
{

	FVector2d InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		SubmarineMesh->AddForce(ElevateForce * InputVector.Y * GetActorUpVector());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Elevating")));
	}
	

}

void APlayerSubmarine::Look_Implementation(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Looking")));
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerSubmarine::TogglePauseMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pause")));
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	
	bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());


	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD());
	if (HUD)
	{
		HUD->SetVisibility(bIsPaused);
	}
	
	if (bIsPaused)
	{
		if (PauseMenu)
			{
				PauseMenu->RemoveFromParent();
				//PauseMenu = nullptr;
			
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fuck")));
			}
		
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Balls")));

		
	}

	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		if (PauseMenuClass)
		{
			PauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuClass);
			if (PauseMenu)
			{
				PauseMenu->AddToViewport();

				
				PlayerController->bShowMouseCursor = true;
				
				
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(PauseMenu->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);

				
			}
		}
	}
	
}

void APlayerSubmarine::RefillOxygen(float Amount)
{
	CurrentOxygen = FMath::Clamp(CurrentOxygen + Amount, 0.0f, MaxOxygen);
}

void APlayerSubmarine::StraightenBoatLevel()
{
/*
	if (SubmarineMesh->GetRelativeRotation().Yaw > 0)
	{
		
		SubmarineMesh -> SetRelativeRotation()
	}
	*/
}


void APlayerSubmarine::InteractWithObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interact")));
}

void APlayerSubmarine::LostGame()
{
	UE_LOG(LogTemp, Display, TEXT("You Lost The Game!"));
}

