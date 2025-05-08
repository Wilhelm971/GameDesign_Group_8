// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzleManager.h"

ACPP_SequencePuzzleManager::ACPP_SequencePuzzleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_SequencePuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			Piece->OnStateChanged.RemoveDynamic(this, &ACPP_SequencePuzzleManager::OnSequencePieceMoved); 
			Piece->OnStateChanged.AddDynamic(this, &ACPP_SequencePuzzleManager::OnSequencePieceMoved);  
		}
	}
	
}

//Checks if puzzle is solved, depending if the activation order mattered or not
void ACPP_SequencePuzzleManager::CheckPuzzleState() 
{
	Super::CheckPuzzleState();

	bool bIsComplete = true;

	if (bDoesCareAboutOrder)
	{
		//Compares in order
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
		//Compares in any order
		for (int i = 0; i < PuzzlePiecesSolution.Num(); i++)
		{
			if (!PuzzlePiecesCurrent.Contains(PuzzlePiecesSolution[i]))
			{
				bIsComplete = false;
				break;
			}
		}
	}
	
	if (bIsComplete)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast(); // Trigger the delegate
		SetActorTickEnabled(false);
		bPuzzleSolved = true;
		
		//Set puzzle pieces as inactive and disables Highlight materials
		for (int32 i = 0; i < PuzzlePieces.Num(); i++)
		{
			PuzzlePieces[i]->SetActorTickEnabled(false);
			PuzzlePieces[i]->bIsActive = false;
		}
	}
	
	
}

//Resets puzzle if solution didn't match
void ACPP_SequencePuzzleManager::ResetPuzzle()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Resetting puzzle"));

	PuzzlePiecesCurrent.Empty();
	

	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			Piece->ResetPiece();

		}

	}
	

}

//Updates puzzle and/or checks if puzzle is completed
void ACPP_SequencePuzzleManager::OnSequencePieceMoved(ACPP_PuzzleBase* ChangedPiece)
{
	
	if (ACPP_SequencePuzzlePiece* MovedPiece = Cast<ACPP_SequencePuzzlePiece>(ChangedPiece))
	{
		if (!PuzzlePiecesCurrent.Contains(MovedPiece->GetPuzzlePieceId()))
		{
			PuzzlePiecesCurrent.Add(MovedPiece->GetPuzzlePieceId());
		}

		if (PuzzlePiecesCurrent.Num() == PuzzlePiecesSolution.Num())
		{
			CheckPuzzleState();
		}
	}
}



