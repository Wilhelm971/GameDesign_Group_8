// Copyright W.V. Koren. All Rights Reserved


#include "PlayerSubmarine.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MeshPaintVisualize.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameHUD.h"
#include "UI/PauseScreenWidget.h"
#include "UI/GameHUDWidget.h"


// Sets default values
APlayerSubmarine::APlayerSubmarine()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxSize = FVector(32.0f, 32.0f, 32.0f);
	
	// Creating the mesh, camera and collision
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box -> SetBoxExtent(BoxSize);
	Box -> SetSimulatePhysics(true);
	Box -> SetEnableGravity(false); // Submarine is underwater
	Box -> SetCollisionProfileName(TEXT("PhysicsActor"));
	
	RootComponent = Box;

	SubmarineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubmarineMesh"));
	SubmarineMesh -> SetupAttachment(RootComponent);
	SubmarineMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(RootComponent);
	SpringArm -> bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;



	// Defining MovementSpeed & Oxygen

	MoveForce = 100000.0f;
	TorqueForce = 4000000.0;
	ElevateForce = 48000.0f;

	MaxOxygen = 100.0f;
	CurrentOxygen = 60.0f;
	OxygenDrainRate = 1.2f;


	
	bIsPaused = false;
	bLostGame = false;

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
/*
			if (HUDWidget)
			{
				if (UGameHUDWidget* GameHUD = Cast<UGameHUDWidget>(HUDWidget))
				{
					float Percent = FMath::Clamp(CurrentOxygen/MaxOxygen, 0.f, 1.f);
					GameHUD -> OxygenPercent = Percent;
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Check");
				}
			}
			*/
			
		}

		if (CurrentOxygen <= 0.0f && bLostGame == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, "Oxygen Drained !");
			bLostGame = true;
			LostGame();
		}
	}


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



void APlayerSubmarine::Move(const FInputActionValue& InputValue)
{
	const float Input = InputValue.Get<float>();

	if (IsValid(Controller))
	{
		if (FMath::Abs(Input) > KINDA_SMALL_NUMBER)
		{
			FVector ForceDirection = GetActorForwardVector().GetSafeNormal();
			FVector ForceToAdd = ForceDirection * Input * MoveForce;
			Box -> AddForce(ForceToAdd);
		}
	}
}

void APlayerSubmarine::Rotate_Implementation(const FInputActionValue& InputValue)
{

	const float Input = InputValue.Get<float>();
	if (IsValid(Controller))
	{
		FVector ForceDirection = GetActorUpVector().GetSafeNormal();
		FVector ForceToAdd = ForceDirection * Input * TorqueForce;
		Box -> AddTorqueInRadians(ForceToAdd);
	}
}

void APlayerSubmarine::Elevate_Implementation(const FInputActionValue& InputValue)
{

	const float Input = InputValue.Get<float>();


	if (IsValid(Controller))
	{
		if (FMath::Abs(Input) > KINDA_SMALL_NUMBER)
		{
			FVector ForceDirection = GetActorUpVector().GetSafeNormal();
			FVector ForceToAdd = ForceDirection * Input * ElevateForce;
			Box -> AddForce(ForceToAdd);
		}
	}
}

void APlayerSubmarine::Look_Implementation(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
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



void APlayerSubmarine::InteractWithObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interact")));
}

void APlayerSubmarine::LostGame()
{
	UE_LOG(LogTemp, Display, TEXT("You Lost The Game!"));
}

