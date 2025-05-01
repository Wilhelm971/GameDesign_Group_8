// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseFishAI.h"
#include "GameFramework/Actor.h"
#include "CPP_SkeletalFish.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_SkeletalFish : public ACPP_BaseFishAI
{
	GENERATED_BODY()
    
public:    
	ACPP_SkeletalFish();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FishAI")
	USkeletalMeshComponent* FishMesh;
};