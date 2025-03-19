// Fill out your copyright notice in the Description page of Project Settings.


#include "Cpp_Player.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACpp_Player::ACpp_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule"));
	Capsule->SetBoxExtent(FVector(75.0f, 60.0f, 70.0f));

	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Capsule;


	SubmarineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubmarineMesh"));
	SubmarineMesh->SetupAttachment(RootComponent);
	SubmarineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	
}

// Called when the game starts or when spawned
void ACpp_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACpp_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem -> AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ACpp_Player::MoveForward);
		Input->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &ACpp_Player::MoveUp);
		
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACpp_Player::Look);
		Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ACpp_Player::Turn);

		
	}

}
// Called every frame
void ACpp_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(RotationVelocity* DeltaTime);
	
}

// Called to bind functionality to input

void ACpp_Player::MoveForward(const FInputActionValue& InputValue)
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Player Movement Input"));

	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);

		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		if (InputVector.Y < 0)
		{
			MovementComponent->AddInputVector(ForwardRotation*0.5*InputVector.Y);

		}
		else
		{
			MovementComponent->AddInputVector(ForwardRotation*InputVector.Y);

		}
	}
}

void ACpp_Player::Turn(const FInputActionValue& InputValue)
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Player Rotate Input"));

	float Value = InputValue.Get<float>();


	
if (FMath::Abs(Value) > 0.1f) // Ignore small input values
{
	RotationVelocity.Yaw += Value;
}
	
}

void ACpp_Player::Look(const FInputActionValue& InputValue)
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Player Look Input"));

	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void ACpp_Player::MoveUp(const FInputActionValue& InputValue)
{
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Player Elevate Input"));

	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);
		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		
		MovementComponent->AddInputVector(ForwardRotation*InputVector.Y);
		
	}

}


