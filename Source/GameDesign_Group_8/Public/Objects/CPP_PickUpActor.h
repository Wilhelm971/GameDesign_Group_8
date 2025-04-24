// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_InteractableObjectBase.h"
#include "Interact_Interface.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableObjectComponent.h"
#include "CPP_PickUpActor.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_PickUpActor : public ACPP_InteractableObjectBase
{
	GENERATED_BODY()

	FTimerHandle DestroyTimerHandle;
	FTimerHandle BounceMoveTimerHandle;
	float ElapsedTime = 0.0f;
	float Duration = 0.5f;
	float BounceHeight = 70.0f;

	bool bShouldMove = false;

	FVector StartLocation;
	FVector TargetLocation;
	
	TWeakObjectPtr<APawn> PlayerTarget;
	
	FVector StartSize;
	FVector TargetSize;

public:	
	// Sets default values for this actor's properties
	ACPP_PickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* CausingActor) override;

	void UpdateMovementAndRotation(float DeltaTime);

	void DestroyObject();

	void IdleMovement(float DeltaTime);

};
