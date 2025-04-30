// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPP_PickUpActor.h"

#include "CPPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_PickUpActor::ACPP_PickUpActor()
{
 	
}

// Called when the game starts or when spawned
void ACPP_PickUpActor::BeginPlay()
{
    Super::BeginPlay();
	
    StartLocation = GetActorLocation();
    StartSize = GetActorScale3D();
    TargetSize = FVector(0, 0, 0);
}

void ACPP_PickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bShouldMove) // Add a flag to control when movement starts
    {
        UpdateMovementAndRotation(DeltaTime);
    }
    else
    {
        IdleMovement(DeltaTime);
    }
    

}

void ACPP_PickUpActor::OnInteract_Implementation(AActor* CausingActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Interacted With Object");
    PrimaryActorTick.bCanEverTick = true;
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    bShouldMove = true;
	
	if ((PlayerTarget = Cast<APawn>(CausingActor)).IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Target is valid"));
		FVector PlayerForward = PlayerTarget->GetActorForwardVector();
		// Set TargetLocation just once, relative to player position.
		TargetLocation = PlayerTarget->GetActorLocation() + (PlayerForward * 50.0f);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Target is not valid"));
	}
	
    ElapsedTime = 0.0f;
    

	// Start animation update
	//GetWorld()->GetTimerManager().SetTimer(BounceMoveTimerHandle, this, &ACPPInteractableObject::UpdateMovementAndRotation, 0.01f, true);

	// Schedule destruction
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACPP_PickUpActor::DestroyObject, Duration, false);

	
}

void ACPP_PickUpActor::UpdateMovementAndRotation(float DeltaTime)
{
    // Increase time smoothly using DeltaTime
    ElapsedTime += DeltaTime;

    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f); // Normalize time (0 to 1)

    if (Alpha >= 1.0f)
    {
        PrimaryActorTick.bCanEverTick = false; // Stop updating after reaching target
    }

    // Smooth Lerp Movement
    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    float BounceOffset = BounceHeight * FMath::Sin(Alpha * PI);
    NewLocation.Z += BounceOffset;
    SetActorLocation(NewLocation);

    // Rotate smoothly
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 20;
    SetActorRotation(NewRotation);

    // Smooth Size Change
    FVector NewScale = FMath::Lerp(StartSize, TargetSize, Alpha);
    SetActorScale3D(NewScale);
}

void ACPP_PickUpActor::DestroyObject()
{
    Destroy();
}

void ACPP_PickUpActor::IdleMovement(float DeltaTime)
{
    ElapsedTime += DeltaTime;


    FVector NewPosition = GetActorLocation();
    float YOffset = NewPosition.Y;


    float ZOffset = 15.0f * FMath::Sin((ElapsedTime * PI) + YOffset);
    NewPosition.Z = StartLocation.Z + ZOffset;
    SetActorLocation(NewPosition);


    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 120.0f * DeltaTime;
    SetActorRotation(NewRotation);

    FVector Size = GetActorScale3D();
    float SizeOffset = 1 + 0.1 * FMath::Sin((ElapsedTime * PI));

    Size = StartSize * SizeOffset;
    SetActorScale3D(Size);
}

 