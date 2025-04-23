// Copyright W.V. Koren. All Rights Reserved


#include "Objects/OxygenBubble.h"

#include "PlayerSubmarine.h"
#include "Components/SphereComponent.h"

// Sets default values
AOxygenBubble::AOxygenBubble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetSphereRadius(100.0f);
	Collision->SetCollisionProfileName(TEXT("OverlapAll"));

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AOxygenBubble::OnOverlap);

	OxygenAmount = 40.0f;

}

// Called when the game starts or when spawned
void AOxygenBubble::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOxygenBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOxygenBubble::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerSubmarine* Sub = Cast<APlayerSubmarine>(OtherActor);
	if (Sub)
	{
		Sub->RefillOxygen(OxygenAmount);
		Destroy();
	}
}

