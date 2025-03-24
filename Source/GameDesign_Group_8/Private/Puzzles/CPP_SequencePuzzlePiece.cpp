// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzlePiece.h"


ACPP_SequencePuzzlePiece::ACPP_SequencePuzzlePiece()
{
	MovementSpeed = 100.0f;
	bCanMove = false;
	bIsActive = true;
	bIsResetting = false;
	PrimaryActorTick.bCanEverTick = true;

}

bool ACPP_SequencePuzzlePiece::GetbCanMove() const
{
	return bCanMove;
}

bool ACPP_SequencePuzzlePiece::GetbIsResetting() const
{
	return bIsResetting;
}

int32 ACPP_SequencePuzzlePiece::GetPuzzlePieceId() const
{
	return PuzzlePieceId;
}


void ACPP_SequencePuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	StaticMesh->SetMaterial(0,InactiveMaterial);
	StartingLocation = GetActorLocation();
	EndingLocation = StartingLocation + FVector(0.0f, 0.0f, -50.0f);
}

void ACPP_SequencePuzzlePiece::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activated Puzzlepiece"));

	Super::Activate();
	if (bIsActive)
	{
		bCanMove = true;
		SetActorTickEnabled(true);

		UE_LOG(LogTemp, Warning, TEXT("bCanMove set to: %d"), bCanMove);

		bIsActive = false;
		StaticMesh->SetMaterial(0, ActiveMaterial);
		EndingLocation = GetActorLocation() + FVector(0.0f, 0.0f, -50.0f);
	}
}

//Function to move the PuzzlePiece when activated
void ACPP_SequencePuzzlePiece::Move(float DeltaTime)
{
	FVector NewPosition = FMath::VInterpTo(GetActorLocation(), EndingLocation, DeltaTime, MovementSpeed);
	SetActorLocation(NewPosition);
        
	if (FVector::Dist(NewPosition, EndingLocation) <= 0.01f)
	{
		SetActorLocation(EndingLocation);
		bCanMove = false;
		SetActorTickEnabled(false);
		
		if (bIsResetting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Resetting piece %d completed"), PuzzlePieceId);
            
			bIsActive = true;        
			bIsResetting = false;    
		}
		else
		{
			OnStateChanged.Broadcast(this);  
			bIsActive = false;
		}
		
	}
}


void ACPP_SequencePuzzlePiece::ResetMovement()
{
	//Check if a piece is moving and stop ResetMovement if so 
	if (bCanMove)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetMovement skipped because piece is currently moving!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ResetMovement started for piece %d"), PuzzlePieceId);

    bIsResetting = true;
    bCanMove = true;
    EndingLocation = StartingLocation;
	
    UE_LOG(LogTemp, Warning, TEXT("EndingLocation: %f"), EndingLocation.Z);

	StaticMesh->SetMaterial(0, InactiveMaterial);
	
	SetActorTickEnabled(true);
	
	bIsActive = false;

}


void ACPP_SequencePuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanMove)
	{
		Move(DeltaTime);
	}

}
