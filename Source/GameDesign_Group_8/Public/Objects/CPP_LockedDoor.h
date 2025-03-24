// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puzzles/CPP_PuzzleManagerBase.h"
#include "CPP_LockedDoor.generated.h"

class APuzzleManager;

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_LockedDoor : public AActor
{
	GENERATED_BODY()

	FVector StartingLocation;
	FVector endingLocation;

	bool bCanOpen = false;

	UPROPERTY(EditAnywhere, Category = "Opening Parameters")
	float OpeningSpeed = 1.0f;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "References")
	ACPP_PuzzleManagerBase* LinkedPuzzleManager; // Set this in the Editor for each door
	
	// Sets default values for this actor's properties
	ACPP_LockedDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenDoor(); // The event triggered when the puzzle is solved

};
