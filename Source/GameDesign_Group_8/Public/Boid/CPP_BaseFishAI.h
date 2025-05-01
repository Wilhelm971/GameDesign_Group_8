// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPP_BaseFishAI.generated.h"

UENUM(BlueprintType)
enum class EFishBehaviorMode : uint8
{
	Wander          UMETA(DisplayName = "Wander"),
	FollowPlayer    UMETA(DisplayName = "Follow Player"),
	FollowSpline    UMETA(DisplayName = "Follow Spline")
};

class USplineComponent;

UCLASS(Abstract)
class GAMEDESIGN_GROUP_8_API ACPP_BaseFishAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_BaseFishAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FishAI")
	FVector CurrentVector;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float MovementSpeed = 100.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float AvoidanceFactor = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float VisionConeMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	bool bCanFollowPlayer = false;

	//SPLINE TEST
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FishAI")
	USplineComponent* SplineToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	AActor* SplineActor;
	
	void SetSplineToFollow(const AActor* NewSplineActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	EFishBehaviorMode CurrentBehaviorMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	bool bLoopAlongSpline;

	float CurrentSplineDistance;

	UPROPERTY(EditAnywhere, Category = "FishAI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SpeedVariation;
    
	UPROPERTY(EditAnywhere, Category = "FishAI")
	float PathDeviation;
	
	int32 SplineDirection;
	
	UPROPERTY(EditAnywhere, Category = "FishAI")
	float InitialSplineOffset;

	float DeviationChangeTimer;
	FVector CurrentDeviation;
	FVector MovementDirection;
	
	//For triggering CheckObstacles
	FTimerHandle TimerHandle;

	//Timers for movement handeling
	float TimeSinceObstacleAvoidance;
	float TimeSinceDirectionChange;
	float DirectionChangeTime;
	float FollowPlayerTimer;
	
	float PlayerDistance;
	float MovementSpeedFactor;

	bool bIsChangingDirection;
	
    FVector RandomDir;
	FVector CurrentAvoidanceDirection;
    
	// Functions
	void CheckObstacles();
	void MoveAndRotate(float DeltaTime);
	void FollowSpline(float DeltaTime);
	void FollowPlayer(float DeltaTime);
	void Wander(float DeltaTime);
    
	// Debug visualization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugRays = true;
};
