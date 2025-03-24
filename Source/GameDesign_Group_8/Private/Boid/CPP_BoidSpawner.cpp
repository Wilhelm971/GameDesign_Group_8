// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid/CPP_BoidSpawner.h"

// Sets default values
ACPP_BoidSpawner::ACPP_BoidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_BoidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_BoidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

