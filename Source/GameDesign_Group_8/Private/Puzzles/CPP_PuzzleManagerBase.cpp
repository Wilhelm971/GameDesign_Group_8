// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_PuzzleManagerBase.h"


// Sets default values
ACPP_PuzzleManagerBase::ACPP_PuzzleManagerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bPuzzleSolved = false;
}

void ACPP_PuzzleManagerBase::CheckPuzzleState()
{
}

// Called when the game starts or when spawned
void ACPP_PuzzleManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_PuzzleManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

