// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact_Interface.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableObjectComponent.h"
#include "CPP_PuzzleBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, ACPP_PuzzleBase*, ChangedPiece);


UCLASS(Abstract)
class GAMEDESIGN_GROUP_8_API ACPP_PuzzleBase : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

protected:
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UCPP_InteractableObjectComponent* InteractableObjectComponent;
	
public:	
	ACPP_PuzzleBase();

	UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Puzzle")
	bool bIsActive;

	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	virtual void Activate();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* CausingActor) override;



};
