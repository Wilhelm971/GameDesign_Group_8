// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interact_Interface.h"
#include "CPP_InteractableObjectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEDESIGN_GROUP_8_API UCPP_InteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"), Category = "Interaction")
	UPrimitiveComponent* CollisionShape;
	
public:
	// Sets default values for this component's properties
	UCPP_InteractableObjectComponent();

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Interaction")
	UMaterialInterface* HighlightMaterial;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	void SetCollisionShape(UPrimitiveComponent* NewCollisionShape);
	
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void Interact(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleHighlight(bool bEnableHighlight);
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					  bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
