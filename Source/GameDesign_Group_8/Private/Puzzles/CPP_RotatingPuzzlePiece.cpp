// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_RotatingPuzzlePiece.h"

// Sets default values
ACPP_RotatingPuzzlePiece::ACPP_RotatingPuzzlePiece()
{
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
	if (!bIsRotating && bIsActive)
	{
		bIsRotating = true;
		CurrentTime = 0.0f;
		StartRotation =	StaticMesh->GetRelativeRotation();
		TargetRotation = StartRotation + FRotator(0, RotationAmount, 0);
		
	}
	
}

//Rotates and notifyes puzzlemanager when rotation is done
void ACPP_RotatingPuzzlePiece::Rotate(float DeltaTime)
{
	FRotator NewRotation = FMath::RInterpConstantTo(StaticMesh->GetRelativeRotation(), TargetRotation, DeltaTime, RotationSpeed);

	StaticMesh->SetRelativeRotation(NewRotation);

	if (NewRotation.Equals(TargetRotation, 1.0f)) 
	{
		bIsRotating = false;
		StaticMesh->SetRelativeRotation(TargetRotation); 
		RotationPosition++;
		if (RotationPosition > RotationMaxCount-1)
		{
			RotationPosition = 0;
			StaticMesh->SetRelativeRotation(FRotator(0, 0, 0));
		}

		OnStateChanged.Broadcast(this);
		OnActivate();
		
	}


		
}


