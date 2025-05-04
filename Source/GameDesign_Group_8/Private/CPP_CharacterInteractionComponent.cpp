// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterInteractionComponent.h"

#include "CPP_InteractableObjectComponent.h"

// Sets default values for this component's properties
UCPP_CharacterInteractionComponent::UCPP_CharacterInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_CharacterInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UCPP_CharacterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCPP_CharacterInteractionComponent::OnBeginOverlap(AActor* CausingActor)
{
	if (CausingActor)
	{
		if (OverlappingActors.Num() == 0)
		{
			bToggleHighlightSwitch = true;
			ToggleHighlightActor();
		}

		OverlappingActors.Add(CausingActor);
	}
}

void UCPP_CharacterInteractionComponent::OnEndOverlap(AActor* CausingActor)
{
	if (CausingActor)
	{
		OverlappingActors.Remove(CausingActor);
		if (OverlappingActors.Num() == 0)
		{
			bToggleHighlightSwitch = false;
			ToggleHighlightActor();
		}

		// Makes sure the highlight is turned off when an object is no longer part of the array
		if (UCPP_InteractableObjectComponent* InteractableComponent = Cast<UCPP_InteractableObjectComponent>(
		CausingActor->GetComponentByClass(UCPP_InteractableObjectComponent::StaticClass())))
		{
			InteractableComponent->ToggleHighlight(false);
		}
	}
}

void UCPP_CharacterInteractionComponent::OnInteract()
{
	if (OverlappingActors.Num() == 0)
	{
		return;
	}

	if (!OverlappingActors.IsValidIndex(ClosestObjectIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("ClosestObjectIndex is invalid. Skipping interaction."));
		return;
	}
	
	if (UCPP_InteractableObjectComponent* InteractableComponent = Cast<UCPP_InteractableObjectComponent>(
	OverlappingActors[ClosestObjectIndex]->GetComponentByClass(UCPP_InteractableObjectComponent::StaticClass())))
	{
		InteractableComponent->Interact(GetOwner());
	}

	
}

void UCPP_CharacterInteractionComponent::ToggleHighlightActor()
{
	if (bToggleHighlightSwitch)
	{
		GetWorld()->GetTimerManager().SetTimer(
		HighlightTimerHandle,												// Handle to manage this timer
			this,																// Object to call the timer function on
			&UCPP_CharacterInteractionComponent::FindNearestInteractableActor,	// Function to call
			HighlightTimerInterval,												// Timer interval
			true																// Looping enabled
		);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HighlightTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Timer stopped."));

	}
}

void UCPP_CharacterInteractionComponent::FindNearestInteractableActor()
{
	float ClosestObjectDistance = 0;

	
	//Loops throuhg array and finds the index of the closest object
	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		float ComparedDistanceValue = FVector::Dist(OverlappingActors[i]->GetActorLocation(),GetOwner()->GetActorLocation());			
		if (i == 0)
		{
			ClosestObjectDistance = ComparedDistanceValue;
			ClosestObjectIndex = i;
			continue;
		}
		
		if(ComparedDistanceValue < ClosestObjectDistance)
		{
			ClosestObjectDistance = ComparedDistanceValue;
			ClosestObjectIndex = i;
		}
		
	}

	//Loops through array and toggles highlight on the closest object
	for (int i = 0; i < OverlappingActors.Num(); i++)	
	{
		//If any of the objects had highlight on, this turns them off
		if (i != ClosestObjectIndex)
		{
			if (UCPP_InteractableObjectComponent* InteractableComponent = Cast<UCPP_InteractableObjectComponent>(
		OverlappingActors[i]->GetComponentByClass(UCPP_InteractableObjectComponent::StaticClass())))
			{
				InteractableComponent->ToggleHighlight(false);
			}
		}

		//Toggles highlight on the closest object
		else if (i == ClosestObjectIndex)
		{
			if (UCPP_InteractableObjectComponent* InteractableComponent = Cast<UCPP_InteractableObjectComponent>(
		OverlappingActors[i]->GetComponentByClass(UCPP_InteractableObjectComponent::StaticClass())))
			{
				InteractableComponent->ToggleHighlight(true);
			}
		}
	}
	
}


