// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CPP_BoidActor.generated.h"

class ACPP_BoidGridManager;

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_BoidActor : public AActor
{
	GENERATED_BODY()
    
public:    
	ACPP_BoidActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid|Environment")
	AActor* BoundaryVolume;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid|Environment", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float BoundaryAvoidanceStrength = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid|Environment")
	float BoundaryMargin = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fish")
	UStaticMeshComponent* FishMesh;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector CurrentVector;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AvoidanceFactor = 5.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FVector EnvironmentMin;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FVector EnvironmentMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float VisionConeMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	bool bCanFollowPlayer = false;

	FTimerHandle TimerHandle;

	//Timers for movement handeling
	float TimeSinceObstacleAvoidance;
	float TimeSinceDirectionChange;
	float DirectionChangeTime;
	
	float FollowPlayerTimer;
	float PlayerDistance;
	float MovementSpeedFactor;

	bool bShouldFollowPlayer;
	bool bIsChangingDirection;
	
    FVector RandomDir;
	FVector CurrentAvoidanceDirection;

	//Target for the boid to follow
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector FollowTarget;
    
	// Functions
	void CheckObstacles();
	void MoveAndRotate(float DeltaTime);
	void KeepInBounds();
    
	// Debug visualization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugRays = true;
};