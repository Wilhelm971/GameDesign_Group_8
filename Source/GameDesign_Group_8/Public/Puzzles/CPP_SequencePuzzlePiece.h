// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puzzles/CPP_PuzzleBase.h"
#include "CPP_SequencePuzzlePiece.generated.h"


UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_SequencePuzzlePiece : public ACPP_PuzzleBase
{
	GENERATED_BODY()

	bool bCanMove;
	bool bIsResetting;
	
	FVector StartingLocation;
	FVector EndingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true ), Category = "Puzzle Piece")
	int32 PuzzlePieceId;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	UMaterialInterface* ActiveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	UMaterialInterface* InactiveMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	float MovementSpeed;
	

	ACPP_SequencePuzzlePiece();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	virtual void Activate() override;

	bool GetbCanMove() const;
	
	bool GetbIsResetting() const;
	
	int32 GetPuzzlePieceId() const;

	void Move(float DeltaTime);

	void ResetMovement();
	
};
