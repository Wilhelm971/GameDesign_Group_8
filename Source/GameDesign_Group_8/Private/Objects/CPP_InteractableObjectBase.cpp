// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPP_InteractableObjectBase.h"

// Sets default values
ACPP_InteractableObjectBase::ACPP_InteractableObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Collision Sphere");
	CollisionSphere->SetupAttachment(StaticMesh);
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	InteractableObjectComponent = CreateDefaultSubobject<UCPP_InteractableObjectComponent>(TEXT("InteractComponent"));

}

void ACPP_InteractableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Sets up overlap events for the InteractableObjectComponent
	if (InteractableObjectComponent && CollisionSphere)
	{
		InteractableObjectComponent->SetCollisionShape(CollisionSphere);
	}
}

void ACPP_InteractableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

