// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPCharacter.h"
//#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values
ACPPCharacter::ACPPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the spring arm and attaches it to the Character root component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	//Creates the camera and attaches it to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	CharacterInteractionComponent = CreateDefaultSubobject<UCPP_CharacterInteractionComponent>(TEXT("CharacterInteractionComponent"));


}

// Called when the game starts or when spawned
void ACPPCharacter::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ACPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPPCharacter::BeginOverlap_Implementation(AActor* CausingActor)
{
	IInteract_Interface::BeginOverlap_Implementation(CausingActor);
	if (CharacterInteractionComponent)
	{
		CharacterInteractionComponent->OnBeginOverlap(CausingActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInteractionComponent is not valid! Check initialization."));
	}
}

void ACPPCharacter::EndOverlap_Implementation(AActor* CausingActor)
{
	IInteract_Interface::EndOverlap_Implementation(CausingActor);
	if (CharacterInteractionComponent)
	{
		CharacterInteractionComponent->OnEndOverlap(CausingActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInteractionComponent is not valid! Check initialization."));
	}
}

// Called to bind functionality to input
void ACPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}

	}
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Move);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Look);

		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Jump);

		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPPCharacter::InteractWithObject);
	}

}

//Add movement based on input
void ACPPCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		//Get forward and right direction of character
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);

		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Sets the movement values
		AddMovementInput(ForwardRotation, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

//Lets the camera move based on input
void ACPPCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}

}

//Calls the jump function based on input
void ACPPCharacter::Jump()
{
	ACharacter::Jump();
}

void ACPPCharacter::InteractWithObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Pressed E");

	//Executes interact functionality inside component
	if (CharacterInteractionComponent)
	{
		CharacterInteractionComponent->OnInteract();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInteractionComponent is not valid! Check initialization."));
	}
}




