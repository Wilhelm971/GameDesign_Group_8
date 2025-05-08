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

	//Set base speed of fish in editor, standard is 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float MovementSpeed = 100.0f;

	//Changes the strength of the avoidance vector that affects how much the fish avoids obstacles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float AvoidanceFactor = 1.0f;

	//The radius of the raycasts the fish uses to "see", higher number means a wider vision cone for the fish
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	float VisionConeMultiplier = 0.5f;

	//If the fish should be able to follow the player if it detects it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	bool bCanFollowPlayer = false;

	//These two variables are used for a dropped behaviour mode for the fish, where you would choose a spline actor in game and make the fish follow it instead of free roaming
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FishAI")
	USplineComponent* SplineToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	AActor* SplineActor;

	//Used to dictate how the fish handles movement in tick function, should not be used in editor anymore, it is used inside the class as part of the tick method to set behaviour internally based on conditions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	EFishBehaviorMode CurrentBehaviorMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FishAI")
	bool bLoopAlongSpline;

	//Adds some small variations in the fish movement during runtime
	UPROPERTY(EditAnywhere, Category = "FishAI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SpeedVariation;
	 
	UPROPERTY(EditAnywhere, Category = "FishAI")
	float PathDeviation;
		
	UPROPERTY(EditAnywhere, Category = "FishAI")
	float InitialSplineOffset;

	// Shows the raycasts used for obstacle avoidance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugRays = true;
	
private:

	//Used for the spline movement, sets splin, and you can change move direction along the spline
	void SetSplineToFollow(const AActor* NewSplineActor);

	//Variables that were meant to be used for spline movement
	float CurrentSplineDistance;
	int32 SplineDirection;
	float DeviationChangeTimer;
	FVector CurrentDeviation;

	//Helper variable used for calculating movement and obstacle avoidance
	FVector MovementDirection;
	
	//For triggering CheckObstacles, instead of checking every tick, which is unnecessary 
	FTimerHandle TimerHandle;

	//Timers to randomly choose another movedirection if no obstacles was avoided after a certain time
	float TimeSinceObstacleAvoidance;
	float TimeSinceDirectionChange;
	float DirectionChangeTime;

	//Timer to check how long a fish has followed a player
	float FollowPlayerTimer;

	//Distance from fish to player
	float PlayerDistance;

	//MovementSpeed multiplier, used when  following player to slow fish down if too close
	float MovementSpeedFactor;

	//Bool used when fish is changing movementdirection
	bool bIsChangingDirection;

	//Variables used to calculate random movement and the vector used to calculate obstacle avoidance
    FVector RandomDir;
	FVector CurrentAvoidanceDirection;

	
	//FUNCTIONS
	
	// Function to add an avoidance vector to movement vector if an obstacle was detected, also sets behaviour to FollowPlayer ig bool was checked in editor and fish detects a player 
	void CheckObstacles();

	//Function that sets the movement for the fish after CheckObstacles and FollowPlayer.
	void MoveAndRotate(float DeltaTime);

	//If splineMovement worked this function would move the fish along set spline, and handle movement
	void FollowSpline(float DeltaTime);

	//Sets movementvector to that of the player direction, and slows fish down when too close
	void FollowPlayer(float DeltaTime);

	//Standard movement behaviour, sets random direction if moving in the same direction for too long, also aligns the fish to the horizon gradually to emulate more natural fish behaviour
	void Wander(float DeltaTime);
    
	
};
