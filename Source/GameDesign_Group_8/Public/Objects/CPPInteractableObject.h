// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact_Interface.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableObjectComponent.h"
#include "Components/SphereComponent.h"
#include "CPPInteractableObject.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPPInteractableObject : public AActor, public IInteract_Interface
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
	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta  = (AllowPrivateAccess = "True"))
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UCPP_InteractableObjectComponent* InteractableObjectComponent;
public:	
	// Sets default values for this actor's properties
	ACPPInteractableObject();

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
