// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_SequencePuzzlePiece.h"
#include "Puzzles/CPP_PuzzleManagerBase.h"
#include "CPP_SequencePuzzleManager.generated.h"


UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_SequencePuzzleManager : public ACPP_PuzzleManagerBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	ACPP_SequencePuzzleManager();

	UPROPERTY(EditInstanceOnly, Category = "Puzzle")
	TArray<ACPP_SequencePuzzlePiece*> PuzzlePieces;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Puzzle")
	TArray<int32> PuzzlePiecesSolution;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle")
	TArray<int32> PuzzlePiecesCurrent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	bool bDoesCareAboutOrder = false;

	virtual void CheckPuzzleState() override;

	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void ResetPuzzle();

	UFUNCTION()
	void OnSequencePieceMoved(ACPP_PuzzleBase* ChangedPiece);
	
};
