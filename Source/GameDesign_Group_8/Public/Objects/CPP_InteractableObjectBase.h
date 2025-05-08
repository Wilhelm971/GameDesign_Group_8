// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableObjectComponent.h"
#include "Components/SphereComponent.h"
#include "CPP_InteractableObjectBase.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_InteractableObjectBase : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

	//Base actor made to be extended either in blueprint or by inheritance in code.
	//Sets up basic interaction through the interactableObjectComponent, Interact_Interface and the CharacterInteractionComponent
	
public:	
	// Sets default values for this actor's properties
	ACPP_InteractableObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta  = (AllowPrivateAccess = "True"))
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UCPP_InteractableObjectComponent* InteractableObjectComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
