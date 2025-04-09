// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPP_PuzzleLockedActor.h"

// Sets default values
ACPP_PuzzleLockedActor::ACPP_PuzzleLockedActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SceneRoot->SetupAttachment(RootComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SceneRoot);
	
}

// Called when the game starts or when spawned
void ACPP_PuzzleLockedActor::BeginPlay()
{
	Super::BeginPlay();

	
	// Bind the OpenDoor function to the OnPuzzleSolved event
	if (LinkedPuzzleManager)
	{
		LinkedPuzzleManager->OnPuzzleSolved.AddDynamic(this, &ACPP_PuzzleLockedActor::PuzzleSolved);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PuzzleManager linked to this door"));
	}
	StartingLocation = GetActorLocation();
	EndingLocation = StartingLocation;
	EndingLocation.Z += MoveHeight;


}



