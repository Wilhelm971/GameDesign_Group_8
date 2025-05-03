// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzleManager.h"

ACPP_SequencePuzzleManager::ACPP_SequencePuzzleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_SequencePuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	// Log initial PuzzlePiecesCurrent state
	UE_LOG(LogTemp, Warning, TEXT("Initial PuzzlePiecesCurrent size: %d"), PuzzlePiecesCurrent.Num());

	if (PuzzlePieces.Num() == 0 || PuzzlePiecesSolution.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APuzzleManager::BeginPlay - PuzzlePieces or Solution array is empty! Make sure to populate it in the editor."));
	}


	//Bind PuzzlePieces to broadcast functionality
	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			Piece->OnStateChanged.RemoveDynamic(this, &ACPP_SequencePuzzleManager::OnSequencePieceMoved); // Remove old bindings
			Piece->OnStateChanged.AddDynamic(this, &ACPP_SequencePuzzleManager::OnSequencePieceMoved);   // Add binding
			UE_LOG(LogTemp, Warning, TEXT("Bound puzzle piece to delegate: %s"), *Piece->GetName());

		}
	}

	// Log PuzzlePiecesSolution for reference
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePiecesSolution:"));
	for (int32 Id : PuzzlePiecesSolution)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), Id);
	}
}

void ACPP_SequencePuzzleManager::CheckPuzzleState() 
{
	Super::CheckPuzzleState();


	UE_LOG(LogTemp, Warning, TEXT("CheckPuzzleState() called"));
	
	bool bIsComplete = true;

	if (bDoesCareAboutOrder)
	{
		// Compare each value in order
		for (int i = 0; i < PuzzlePiecesSolution.Num(); i++)
		{
			if (PuzzlePiecesCurrent[i] != PuzzlePiecesSolution[i])
			{
				bIsComplete = false;
				break;
			}
		}
	}
	else
	{
		// Compare each value in any order
		for (int i = 0; i < PuzzlePiecesSolution.Num(); i++)
		{
			if (!PuzzlePiecesCurrent.Contains(PuzzlePiecesSolution[i]))
			{
				bIsComplete = false;
				break;
			}
		}
	}
	
	// If solved, notify listeners
	if (bIsComplete)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast(); // Trigger the delegate
		SetActorTickEnabled(false);
	}

	else
	{
		// Log before resetting
		UE_LOG(LogTemp, Warning, TEXT("Solution incorrect, resetting puzzle."));
		ResetPuzzle();
	}
	
}

//Resets puzzle if solution didn't match
void ACPP_SequencePuzzleManager::ResetPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Resetting puzzle!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Resetting puzzle"));

	PuzzlePiecesCurrent.Empty();
	

	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			Piece->ResetPiece();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not reset Piece, was found moving: %d"),Piece->GetPuzzlePieceId());

		}

	}
	

}

//Updates puzzle and/or checks if puzzle is completed
void ACPP_SequencePuzzleManager::OnSequencePieceMoved(ACPP_PuzzleBase* ChangedPiece)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSequencePieceMoved called for: %s"), *ChangedPiece->GetName());
	
	if (ACPP_SequencePuzzlePiece* MovedPiece = Cast<ACPP_SequencePuzzlePiece>(ChangedPiece))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Moved Piece ID:%s"), *MovedPiece->GetName());

		// Ensure no duplicates
		if (!PuzzlePiecesCurrent.Contains(MovedPiece->GetPuzzlePieceId()))
		{
			PuzzlePiecesCurrent.Add(MovedPiece->GetPuzzlePieceId());
			UE_LOG(LogTemp, Warning, TEXT("Added piece to current sequence: %d"), MovedPiece->GetPuzzlePieceId());
		}

		// Make sure it's only checking when enough pieces have moved
		if (PuzzlePiecesCurrent.Num() == PuzzlePiecesSolution.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Checking puzzle state!"));
			CheckPuzzleState();
		}
	}
}



