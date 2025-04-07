// Copyright W.V. Koren. All Rights Reserved


#include "PlayerSubmarine.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "UI/InGameHUD.h"
#include "UI/GameHUDWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"


// Sets default values
APlayerSubmarine::APlayerSubmarine()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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

	MoveForce = 30000.0f;
	TorqueForce = 200000.0f;
	ElevateForce = 15000.0f;

	MaxOxygen = 100.0f;
	CurrentOxygen = 60.0f;
	OxygenDrainRate = 1.0f;

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


/*
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		CurrentOxygen -= OxygenDrainRate * DeltaTime;
		CurrentOxygen = FMath::Clamp(CurrentOxygen, 0.0f, MaxOxygen);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
		if (HUDWidgetClass)
		{
			PauseMenu = CreateWidget<UPauseScreenWidget>(PlayerController, PauseMenuClass);
			if (HUDWidget)
			{
				//HUDWidget->UpdateOxygen(CurrentOxygen, MaxOxygen);
			}
			
		}

		if (CurrentOxygen <= 0.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, "Oxygen Drained !");
		}
	}

	//StraightenBoatLevel();
*/
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

	}
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Move);
		Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Rotate);
		Input->BindAction(ElevateAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Elevate);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::Look);
		
		Input->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerSubmarine::TogglePauseMenu);
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerSubmarine::InteractWithObject);
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
	

	if (bIsPaused)
	{
		if (PauseMenu)
			{
				PauseMenu->RemoveFromParent();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fuck")));
			}
		
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Balls")));


		if (HUDWidget)
		{
			HUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
	
		bIsPaused = false;
	}

	else
	{
		if (PauseMenuClass)
		{
			PauseMenu = CreateWidget<UPauseScreenWidget>(PlayerController, PauseMenuClass);
			if (PauseMenu)
			{
			
		
				PauseMenu->AddToViewport();

				
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				PlayerController->bShowMouseCursor = true;
				
				
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(PauseMenu->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);


				if ( HUDWidgetClass)
				{
					HUDWidget = CreateWidget<UShellWidget>(GetWorld(), HUDWidgetClass);
					if (HUDWidget)
					{
						HUDWidget-> SetVisibility(ESlateVisibility::Hidden);
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ballsssss")));
					}
				}
	
				
				bIsPaused = true;
				
			}
		}
	}
	
}

void APlayerSubmarine::RefillOxygen(float Amount)
{
	//CurrentOxygen = FMath::Clamp(CurrentOxygen + Amount, 0.0f, MaxOxygen);
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
