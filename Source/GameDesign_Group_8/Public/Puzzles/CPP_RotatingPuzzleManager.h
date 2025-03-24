// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PuzzleManagerBase.h"
#include "CPP_RotatingPuzzlePiece.h"
#include "GameFramework/Actor.h"
#include "CPP_RotatingPuzzleManager.generated.h"



UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_RotatingPuzzleManager : public ACPP_PuzzleManagerBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "PuzzleManager")
	TArray<ACPP_RotatingPuzzlePiece*> PuzzlePieces; 
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "PuzzleManager")
	TArray<int32> PuzzlePiecesSolution;

	virtual void CheckPuzzleState() override;
	
	UFUNCTION()
	void OnPuzzlePieceRotated(ACPP_PuzzleBase* ChangedPiece);

	// Sets default values for this actor's properties
	ACPP_RotatingPuzzleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
