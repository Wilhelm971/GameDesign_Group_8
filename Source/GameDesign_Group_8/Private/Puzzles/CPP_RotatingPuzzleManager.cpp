// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_RotatingPuzzleManager.h"


void ACPP_RotatingPuzzleManager::CheckPuzzleState()
{
	bool bIsSolved = true;

	// Check if puzzle pieces match the solution
	for (int32 i = 0; i < PuzzlePieces.Num(); i++)
	{
		if (PuzzlePieces[i]->RotationPosition != PuzzlePiecesSolution[i])
		{
			bIsSolved = false;
			break;
		}
	}

	// If puzzle is solved then it notifies all actors listening to the event delegate
	if (bIsSolved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast();
		bPuzzleSolved = true;
		
		//Set puzzle pieces as inactive and disables Highlight materials
		for (int32 i = 0; i < PuzzlePieces.Num(); i++)
		{
			if (PuzzlePieces[i])
			{
				PuzzlePieces[i]->bIsActive = false;
				if (UCPP_InteractableObjectComponent* InteractableComponent = PuzzlePieces[i]->FindComponentByClass<UCPP_InteractableObjectComponent>())
				{
					InteractableComponent->SetHighlightMaterial(nullptr);
				}
			}
		}
		
		PrimaryActorTick.bCanEverTick = false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Puzzle Not Solved Yet."));
	}

}

//Checks puzzle every time a piece is interacted with
void ACPP_RotatingPuzzleManager::OnPuzzlePieceRotated(ACPP_PuzzleBase* ChangedPiece)
{
	if (ACPP_RotatingPuzzlePiece* RotatingPiece = Cast<ACPP_RotatingPuzzlePiece>(ChangedPiece))
	{
		CheckPuzzleState();
	}

}


ACPP_RotatingPuzzleManager::ACPP_RotatingPuzzleManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_RotatingPuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (ACPP_RotatingPuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			// Bind CheckPuzzleState to the OnRotationChanged delegate
			Piece->OnStateChanged.AddDynamic(this, &ACPP_RotatingPuzzleManager::OnPuzzlePieceRotated);
		}
	}

}

// Called every frame
void ACPP_RotatingPuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

