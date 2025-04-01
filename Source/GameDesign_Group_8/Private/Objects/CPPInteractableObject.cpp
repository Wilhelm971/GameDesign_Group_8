// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPPInteractableObject.h"

#include "CPPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPPInteractableObject::ACPPInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	//StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	//StaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Collision Sphere");
	CollisionSphere->SetupAttachment(StaticMesh);
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	InteractableObjectComponent = CreateDefaultSubobject<UCPP_InteractableObjectComponent>(TEXT("InteractComponent"));
}

// Called when the game starts or when spawned
void ACPPInteractableObject::BeginPlay()
{
    Super::BeginPlay();

	//Passes the collision shape to the component for logic handeling of Begin and End Overlap
	if (InteractableObjectComponent && CollisionSphere)
	{
		InteractableObjectComponent->SetCollisionShape(CollisionSphere);
	}

    StartLocation = GetActorLocation();
    StartSize = GetActorScale3D();
    TargetSize = FVector(0, 0, 0);
}

void ACPPInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bShouldMove) // Add a flag to control when movement starts
    {
        UpdateMovementAndRotation(DeltaTime);
    }
    else
    {
        IdleMovement(DeltaTime);
    }
    

}

void ACPPInteractableObject::OnInteract_Implementation(AActor* CausingActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Interacted With Object");
    PrimaryActorTick.bCanEverTick = true;
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    bShouldMove = true;
	
	if ((PlayerTarget = Cast<APawn>(CausingActor)).IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Target is valid"));
		FVector PlayerForward = PlayerTarget->GetActorForwardVector();
		// Set TargetLocation just once, relative to player position.
		TargetLocation = PlayerTarget->GetActorLocation() + (PlayerForward * 50.0f);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Target is not valid"));
	}
	
    ElapsedTime = 0.0f;
    

	// Start animation update
	//GetWorld()->GetTimerManager().SetTimer(BounceMoveTimerHandle, this, &ACPPInteractableObject::UpdateMovementAndRotation, 0.01f, true);

	// Schedule destruction
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACPPInteractableObject::DestroyObject, Duration, false);

	
}

void ACPPInteractableObject::UpdateMovementAndRotation(float DeltaTime)
{
    // Increase time smoothly using DeltaTime
    ElapsedTime += DeltaTime;

    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f); // Normalize time (0 to 1)

    if (Alpha >= 1.0f)
    {
        PrimaryActorTick.bCanEverTick = false; // Stop updating after reaching target
    }

    // Smooth Lerp Movement
    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
    float BounceOffset = BounceHeight * FMath::Sin(Alpha * PI);
    NewLocation.Z += BounceOffset;
    SetActorLocation(NewLocation);

    // Rotate smoothly
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 20;
    SetActorRotation(NewRotation);

    // Smooth Size Change
    FVector NewScale = FMath::Lerp(StartSize, TargetSize, Alpha);
    SetActorScale3D(NewScale);
}

void ACPPInteractableObject::DestroyObject()
{
    Destroy();
}

void ACPPInteractableObject::IdleMovement(float DeltaTime)
{
    ElapsedTime += DeltaTime;


    FVector NewPosition = GetActorLocation();
    float YOffset = NewPosition.Y;


    float ZOffset = 15.0f * FMath::Sin((ElapsedTime * PI) + YOffset);
    NewPosition.Z = StartLocation.Z + ZOffset;
    SetActorLocation(NewPosition);


    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += 120.0f * DeltaTime;
    SetActorRotation(NewRotation);

    FVector Size = GetActorScale3D();
    float SizeOffset = 1 + 0.1 * FMath::Sin((ElapsedTime * PI));

    Size = StartSize * SizeOffset;
    SetActorScale3D(Size);
}

 