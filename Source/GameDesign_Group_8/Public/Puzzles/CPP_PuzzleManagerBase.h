// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_PuzzleManagerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_PuzzleManagerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_PuzzleManagerBase();

	UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
	FOnPuzzleSolved OnPuzzleSolved;

	virtual void CheckPuzzleState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
