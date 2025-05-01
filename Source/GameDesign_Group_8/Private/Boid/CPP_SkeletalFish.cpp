#include "Boid/CPP_SkeletalFish.h"
#include "Components/SkeletalMeshComponent.h"


ACPP_SkeletalFish::ACPP_SkeletalFish()
{
    
    FishMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FishMesh"));
    FishMesh->SetupAttachment(RootComponent);
    FishMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FishMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    FishMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}



