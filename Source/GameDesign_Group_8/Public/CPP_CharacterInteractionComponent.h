// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact_Interface.h"
#include "Components/ActorComponent.h"
#include "CPP_CharacterInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEDESIGN_GROUP_8_API UCPP_CharacterInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

	bool bToggleHighlightSwitch = false;
	
	int32 ClosestObjectIndex = 0;

	FTimerHandle HighlightTimerHandle;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Highlight")
	float HighlightTimerInterval = 0.1f;
	
	UCPP_CharacterInteractionComponent();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnBeginOverlap(AActor* CausingActor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnEndOverlap(AActor* CausingActor);

	void ToggleHighlightActor();

	void FindNearestInteractableActor();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnInteract();
	
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

};


