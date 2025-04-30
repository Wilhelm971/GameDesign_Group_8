// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InteractableObjectComponent.h"

#include "Interact_Interface.h"
//#include "InterchangeResult.h"
//#include "ToolWidgetsSlateTypes.h"
#include "GameFramework/Actor.h"
//#include "Components/SphereComponent.h"



// Sets default values for this component's properties
UCPP_InteractableObjectComponent::UCPP_InteractableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CollisionShape = nullptr;
	HighlightMaterial = nullptr;

}

void UCPP_InteractableObjectComponent::SetHighlightMaterial(UMaterialInterface* NewMaterial)
{
	HighlightMaterial = NewMaterial;
}

// Called when the game starts
void UCPP_InteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCPP_InteractableObjectComponent::SetCollisionShape(UPrimitiveComponent* NewCollisionShape)
{
	if (NewCollisionShape)
	{
		CollisionShape = NewCollisionShape;
		CollisionShape->SetGenerateOverlapEvents(true);
		
		// Ignore all channels by default, and only enable overlaps for ECC_Pawn
		CollisionShape->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionShape->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
               
		// Optional: Set the general collision profile for debugging consistency
		CollisionShape->SetCollisionProfileName(TEXT("Custom"));


		// Bind dynamic events
		CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &UCPP_InteractableObjectComponent::BeginOverlap);
		CollisionShape->OnComponentEndOverlap.AddDynamic(this, &UCPP_InteractableObjectComponent::EndOverlap);

		UE_LOG(LogTemp, Log, TEXT("Collision Component Set: %s"), *CollisionShape->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid collision component provided!"));
	}

}

void UCPP_InteractableObjectComponent::Interact(AActor* InstigatorActor)
{
	FString DebugMessage = FString::Printf(TEXT("Interacting with: %s"), *InstigatorActor->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DebugMessage);

	if (GetOwner()->GetClass()->ImplementsInterface(UInteract_Interface::StaticClass()))
	{
		IInteract_Interface::Execute_OnInteract(GetOwner(), InstigatorActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object %s does not implement IInteract_Interface"), *GetOwner()->GetName());
	}

}

void UCPP_InteractableObjectComponent::ToggleHighlight(bool bEnableHighlight)
{
	UMeshComponent* MeshComponent = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass()));
	if (MeshComponent)
	{
		if (bEnableHighlight)
		{
			MeshComponent->SetOverlayMaterial(HighlightMaterial);
		}
		else if (!bEnableHighlight)
		{
			MeshComponent->SetOverlayMaterial(nullptr);
		}
	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No MeshComponent found on %s"), *GetOwner()->GetName());
	}
}

void UCPP_InteractableObjectComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
	if (OtherActor && OtherActor != this->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor is in"));
		if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteract_Interface::StaticClass()))
		{
			IInteract_Interface::Execute_BeginOverlap(OtherActor, GetOwner());
			UE_LOG(LogTemp, Warning, TEXT("Interface found, calling Begin Overlap: "));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OtherActor does not implement Interact_Interface"));
		}

	}
	
}

void UCPP_InteractableObjectComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapping ended"));

	if (OtherActor && OtherActor != this->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor left"));
		if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteract_Interface::StaticClass()))
		{
			IInteract_Interface::Execute_EndOverlap(OtherActor, GetOwner());
			UE_LOG(LogTemp, Warning, TEXT("Interface found, calling Begin Overlap: "));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OtherActor does not implement Interact_Interface"));
		}

	}
}