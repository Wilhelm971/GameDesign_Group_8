// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPSubmarineTest.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values

// Called when the game starts or when spawned

ACPPSubmarineTest::ACPPSubmarineTest()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
	Capsule = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule"));
	Capsule->SetBoxExtent(FVector(75.0f, 60.0f, 70.0f));

	Capsule->SetCollisionProfileName(TEXT("Pawn"));

	SubmarineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubmarineMesh"));
	SubmarineMesh->SetupAttachment(RootComponent);
	SubmarineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = SubmarineMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	CharacterInteractionComponent = CreateDefaultSubobject<UCPP_CharacterInteractionComponent>(TEXT("CharacterInteractionComponent"));

	bUseControllerRotationYaw = false;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	
}

void ACPPSubmarineTest::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent->Acceleration = MoveAcceleration;
	MovementComponent->Deceleration = MoveDeceleration;
	//MovementComponent->TurningBoost = MoveTurningBoost;
	MovementComponent->MaxSpeed = MoveMaxSpeed;
	
}

void ACPPSubmarineTest::BeginOverlap_Implementation(AActor* CausingActor)
{
	if (CharacterInteractionComponent)
	{
		CharacterInteractionComponent->OnBeginOverlap(CausingActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInteractionComponent is not valid! Check initialization."));
	}

}

void ACPPSubmarineTest::EndOverlap_Implementation(AActor* CausingActor)
{
	if (CharacterInteractionComponent)
	{
		CharacterInteractionComponent->OnEndOverlap(CausingActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterInteractionComponent is not valid! Check initialization."));
	}

}


void ACPPSubmarineTest::Elevate(const FInputActionValue& InputValue) 
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);
		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		//AddMovementInput(ForwardRotation, InputVector.Y);
		MovementComponent->AddInputVector(ForwardRotation*ElevationSpeed*InputVector.Y);
		//AddMovementInput(RightDirection, InputVector.X);
		
	}
	
}



void ACPPSubmarineTest::InteractWithObject() 
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

void ACPPSubmarineTest::Move_Implementation(const FInputActionValue& InputValue) 
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);

		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//AddMovementInput(ForwardRotation, InputVector.Y);
		if (InputVector.Y < 0)
		{
			MovementComponent->AddInputVector(ForwardRotation*0.4*InputVector.Y);

		}
		else
		{
			MovementComponent->AddInputVector(ForwardRotation*InputVector.Y);

		}
		//AddMovementInput(RightDirection, InputVector.X);
	}
}

void ACPPSubmarineTest::AdjustPitch()
{
	// Get vertical velocity (Z component)
	float VerticalVelocity = GetVelocity().Z;

	// Use the Z velocity to directly adjust pitch
	// You can adjust the multiplier to control the sensitivity of pitch change
	float PitchChange = VerticalVelocity * 0.004f;  // Adjust multiplier for desired pitch intensity

	// Optionally, clamp the pitch value to avoid excessive rotation
	CurrentPitch += PitchChange;

	// Clamp the pitch value to a range, e.g., -30 to 30 degrees
	CurrentPitch = FMath::Clamp(CurrentPitch, -10.0f, 10.0f);

	// Get the current actor rotation
	FRotator CurrentRotation = GetActorRotation();

	// If vertical movement is near zero, smoothly return pitch to 0
	if (FMath::Abs(VerticalVelocity) < 100)
	{
		// Smoothly interpolate the pitch back to 0 when no vertical movement is detected
		CurrentPitch = FMath::FInterpTo(CurrentPitch, 0.0f, GetWorld()->GetDeltaSeconds(), 2.0f); // Adjust the speed of return
	}

	// Set only the pitch while keeping yaw and roll the same
	CurrentRotation.Pitch = CurrentPitch;
	CurrentRotation.Roll = 0.0f;  // Ensure the roll stays at zero to prevent unintended rotation

	// Set the actor rotation with the updated pitch
	SetActorRotation(CurrentRotation);
	
}

void ACPPSubmarineTest::Rotate_Implementation(const FInputActionValue& InputValue)
{
	float Value = InputValue.Get<float>();

	if (FMath::Abs(Value) > 0.1f) // Ignore small input values
	{
		float RotationAcceleration = RotationSensitivity; // Adjust for sensitivity
		RotationVelocity.Yaw += Value * RotationAcceleration; 
	}
}
	



void ACPPSubmarineTest::Look_Implementation(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void ACPPSubmarineTest::FreeLook()
{
	bIsFreeLooking =true;
}

void ACPPSubmarineTest::UnFreeLook()
{
	bIsFreeLooking =false;

}

float ACPPSubmarineTest::getMoveMaxSpeed()
{
	return MoveMaxSpeed;
}

void ACPPSubmarineTest::RotateToCamera(float DeltaTime)
{

	if (IsValid(Controller))
	{
		FRotator CamereaRotation = Controller->GetControlRotation();
		FRotator TargetRotation = FRotator(0.f, CamereaRotation.Yaw, 0);

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, ReturnToCameraSpeed);

		if (!bIsFreeLooking)
		{
			SetActorRotation(NewRotation);

		}

		
	}
	
}


// Called to bind functionality to input
void ACPPSubmarineTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Move);
		Input->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Rotate);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Look);

		Input->BindAction(ElevateAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Elevate);
		Input->BindAction(FreeLookAction, ETriggerEvent::Started, this, &ACPPSubmarineTest::FreeLook);
		Input->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &ACPPSubmarineTest::UnFreeLook);

		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::InteractWithObject);
	}

}



// Called every frame
void ACPPSubmarineTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply accumulated rotational velocity
	AddActorLocalRotation(RotationVelocity * DeltaTime);
	
	AdjustPitch();

	RotateToCamera(DeltaTime);

	// Use exponential decay for smoother damping
	float DecayFactor = FMath::Exp(-RotationDamping * DeltaTime);
	RotationVelocity *= DecayFactor; // Exponential decay for smooth slowdown
}
