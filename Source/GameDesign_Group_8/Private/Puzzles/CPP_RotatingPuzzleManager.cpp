// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_RotatingPuzzleManager.h"


void ACPP_RotatingPuzzleManager::CheckPuzzleState()
{
	bool bIsSolved = true;

	//Check if solution array is of same length as puzzlepieces
	if (PuzzlePieces.Num() != PuzzlePiecesSolution.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("APuzzleManager::BeginPlay - PuzzlePieces and PuzzlePiecesSolution have mismatched lengths!"));
	}


	// Check if puzzle pieces match the solution
	for (int32 i = 0; i < PuzzlePieces.Num(); i++)
	{
		if (PuzzlePieces[i]->RotationPosition != PuzzlePiecesSolution[i])
		{
			bIsSolved = false;
			break;
		}
	}

	// If solved, notify listeners
	if (bIsSolved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast(); // Trigger the delegate
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

void ACPP_RotatingPuzzleManager::OnPuzzlePieceRotated(ACPP_PuzzleBase* ChangedPiece)
{
	if (ACPP_RotatingPuzzlePiece* RotatingPiece = Cast<ACPP_RotatingPuzzlePiece>(ChangedPiece))
	{
		UE_LOG(LogTemp, Log, TEXT("Puzzle Piece Rotated: %s"), *RotatingPiece->GetName());
		// Call CheckPuzzleState whenever a piece's rotation changes
		CheckPuzzleState();
	}

}


// Sets default values
ACPP_RotatingPuzzleManager::ACPP_RotatingPuzzleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_RotatingPuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Checks that PuzzlePieces is populated
	if (PuzzlePieces.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APuzzleManager::BeginPlay - PuzzlePieces array is empty! Make sure to populate it in the editor."));
	}

	// Bind each puzzle piece's delegate to the CheckPuzzleState function
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

