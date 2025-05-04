// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PuzzleBase.h"
#include "GameFramework/Actor.h"
#include "CPP_RotatingPuzzlePiece.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_RotatingPuzzlePiece : public ACPP_PuzzleBase

{
	GENERATED_BODY()
	
	bool bIsRotating;
	float CurrentTime;
	float RotationTime;
	
	UPROPERTY(EditAnywhere, Category = "Puzzle")
	float RotationSpeed;
	
	FRotator StartRotation;
	FRotator TargetRotation;
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Puzzle")
	int32 RotationPosition;

	//How many rotations to rotate the piece fully
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle")
	int32 RotationMaxCount;

	double RotationAmount;
	
	// Sets default values for this actor's properties
	ACPP_RotatingPuzzlePiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;
	
	void Rotate(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle")
	void OnActivate();

};
