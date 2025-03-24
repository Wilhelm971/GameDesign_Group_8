// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_RotatingPuzzlePiece.h"

// Sets default values
ACPP_RotatingPuzzlePiece::ACPP_RotatingPuzzlePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsRotating = false;
	CurrentTime= 0.0f;
	RotationTime = 1.0f;
	RotationPosition = 0;
	RotationSpeed = 10.0f;
	RotationMaxCount = 4;
	
}

// Called when the game starts or when spawned
void ACPP_RotatingPuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_RotatingPuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the actor if Activate() is triggered
	if (bIsRotating)
	{
		Rotate(DeltaTime);
	}
	

}

void ACPP_RotatingPuzzlePiece::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Activate() called for RotatingPuzzlePiece"));
	RotationAmount = 360/RotationMaxCount;
	//Starts rotation function if it's not already rotating
	if (!bIsRotating)
	{
		bIsRotating = true;
		CurrentTime = 0.0f;
		StartRotation = GetActorRotation();
		TargetRotation = StartRotation + FRotator(0, RotationAmount, 0);
		
	}
}

void ACPP_RotatingPuzzlePiece::Rotate(float DeltaTime)
{
	// Interpolate using RInterpConstantTo for constant rotation speed
	FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

	// Apply the new rotation
	SetActorRotation(NewRotation);

	// Stop rotating when close enough to the target
	if (NewRotation.Equals(TargetRotation, 1.0f)) // 1 degree tolerance
	{
		bIsRotating = false;
		SetActorRotation(TargetRotation); // Ensure exact match
		RotationPosition++;
		if (RotationPosition > RotationMaxCount-1)
		{
			RotationPosition = 0;
			SetActorRotation(FRotator(0, 0, 0));
		}

		OnStateChanged.Broadcast(this);

		
	}


		
}


