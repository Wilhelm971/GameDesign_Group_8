// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPP_LockedDoor.h"

#include "FrameTypes.h"
#include "MaterialHLSLTree.h"

// Sets default values
ACPP_LockedDoor::ACPP_LockedDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SceneRoot->SetupAttachment(RootComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SceneRoot);
	
}

// Called when the game starts or when spawned
void ACPP_LockedDoor::BeginPlay()
{
	Super::BeginPlay();

	
	// Bind the OpenDoor function to the OnPuzzleSolved event
	if (LinkedPuzzleManager)
	{
		LinkedPuzzleManager->OnPuzzleSolved.AddDynamic(this, &ACPP_LockedDoor::OpenDoor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PuzzleManager linked to this door"));
	}
	StartingLocation = GetActorLocation();
	endingLocation = StartingLocation;
	endingLocation.Z += 100.0f;


}

// Called every frame
void ACPP_LockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanOpen)
	{
		FVector NewPosition = FMath::VInterpTo(GetActorLocation(),endingLocation, DeltaTime, OpeningSpeed);
		SetActorLocation(NewPosition);
		if (NewPosition == endingLocation)
		{
			bCanOpen = false;
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

void ACPP_LockedDoor::OpenDoor()
{
	bCanOpen = true;
}

