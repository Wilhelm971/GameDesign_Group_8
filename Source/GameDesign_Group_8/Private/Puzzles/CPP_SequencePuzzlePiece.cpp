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
	
}

//Made to be exteded in blueprint
void ACPP_SequencePuzzlePiece::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activated Puzzlepiece"));

	Super::Activate();
	if (bIsActive)
	{
		
		bIsActive = false;
		
		OnActivated();
	}
}

void ACPP_SequencePuzzlePiece::OnActivated_Implementation()
{
	OnStateChanged.Broadcast(this);  
}

//Made to be exteded in blueprint
void ACPP_SequencePuzzlePiece::ResetPiece_Implementation()
{
	bIsActive = true;

}
	

void ACPP_SequencePuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
