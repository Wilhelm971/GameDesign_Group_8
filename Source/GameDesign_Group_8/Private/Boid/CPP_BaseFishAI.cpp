// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid/CPP_BaseFishAI.h"
#include "Components/SplineComponent.h"

ACPP_BaseFishAI::ACPP_BaseFishAI()
{
    PrimaryActorTick.bCanEverTick = true;
    
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    
    CurrentVector = FVector::ZeroVector;
    CurrentAvoidanceDirection = FVector::ZeroVector;
    RandomDir = FVector::ZeroVector;
    
    TimeSinceObstacleAvoidance = 0.0f;
    TimeSinceDirectionChange = 0.0f;
    FollowPlayerTimer = 0.0f;
    
    PlayerDistance = 1000000;
    MovementSpeedFactor = 1.0f;

    //SPLINE TEST
    //Not currently in use as splinemovement is unfinished
    CurrentBehaviorMode = EFishBehaviorMode::Wander;
    SplineToFollow = nullptr;
    CurrentSplineDistance += InitialSplineOffset;
    SplineDirection = 1;
    SpeedVariation = 0.2f;
    DeviationChangeTimer = 0.0f;
    CurrentDeviation = FVector::ZeroVector;
    MovementDirection = FVector::ZeroVector;
}

void ACPP_BaseFishAI::BeginPlay()
{
    Super::BeginPlay();

    //Sets up movementvector for fish
    CurrentVector = GetActorForwardVector().GetSafeNormal() * MovementSpeed;

    float RandomDelay = FMath::FRandRange(0.0f, 0.1f);

    //Sets spine from blueprint actor
    if (SplineActor)
    {
        SetSplineToFollow(SplineActor);

    }
    
    // Start obstacle avoidance timer
    GetWorld()->GetTimerManager().SetTimer(
         TimerHandle,
         this,
         &ACPP_BaseFishAI::CheckObstacles,
         0.1f,
         true,
         RandomDelay
     );
}

void ACPP_BaseFishAI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    Super::EndPlay(EndPlayReason);
}

//Used to assingn spline, if spline movement was to be used
void ACPP_BaseFishAI::SetSplineToFollow(const AActor* NewSplineActor)
{
    if (NewSplineActor)
    {
        SplineToFollow = Cast<USplineComponent>(NewSplineActor->GetComponentByClass(USplineComponent::StaticClass()));
    }
    else
    {
        SplineToFollow = nullptr;
    }
}

//Main method that handles obstacle avoidance
//Casts 5 raycasts and checks for any static objects that it should avoid, then it picks the shortes raycast, and uses the hitnormal to calculate an avoidance vector to lerp with movement vector
//Set on a timerhandle to not call every tick
//Also checks for players, if found it will set mode to follow player
void ACPP_BaseFishAI::CheckObstacles()
{
    //Set up so raycast distance scales with speed, so the faster fish gets more reaction time to avoid
    const float RaycastDistance = 150.0f * (MovementSpeed / 100.0f) + 100; 
    FVector CurrentLocation = GetActorLocation();
    FVector CurrentDirection = CurrentVector.GetSafeNormal();
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); 

    //Set up ray casts by using vectors and a visionconemultiplier that can be used to increase or decrease "vision" radius of raycasts
    TArray<FVector> Directions;
    Directions.Add(CurrentDirection);
    
    FVector RightVector = FVector::CrossProduct(CurrentDirection, FVector::UpVector).GetSafeNormal();
    FVector UpVector = FVector::CrossProduct(RightVector, CurrentDirection).GetSafeNormal();

    Directions.Add((CurrentDirection + RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection + UpVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - UpVector * VisionConeMultiplier).GetSafeNormal());
    
    // Performs the raycasts and checks for any objects or players
    FVector AvoidanceVector = FVector::ZeroVector;
    float ClosestHitDistance = RaycastDistance;
    bool bObstacleDetected = false;
    
    for (const FVector& Dir : Directions)
    {
        FHitResult Hit;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit,
            CurrentLocation,
            CurrentLocation + Dir.GetSafeNormal() * RaycastDistance,
            ECC_WorldStatic,
            QueryParams
        );

        // If player is detected, switch to follow player mode if bool is set to true
        if (bCanFollowPlayer && Hit.GetActor() && Hit.GetActor()->IsA(APawn::StaticClass()))
        {
            CurrentBehaviorMode = EFishBehaviorMode::FollowPlayer;
        }
        
        if (bHit)
        {
            TimeSinceObstacleAvoidance = 0.0f;
            bObstacleDetected = true;
            AvoidanceVector = Hit.Normal;
        
            if (Hit.Distance < ClosestHitDistance)
            {
                ClosestHitDistance = Hit.Distance;
            }
        
            // If Debug is checked in editor, this visualizes the raycasts that hit targets
            if (bShowDebugRays)
            {
                DrawDebugLine(
                    GetWorld(),
                    CurrentLocation,
                    Hit.Location,
                    FColor::Red,
                    false,       
                    0.0f,        
                    0,           
                    3.0f         
                );
            }
        }
        else if (bShowDebugRays)
        {
            // Green rays for no hit
            DrawDebugLine(
                GetWorld(),
                CurrentLocation,
                CurrentLocation + Dir * RaycastDistance,
                FColor::Green,
                false, 
                0.0f,     
                0, 
                3.0f       
            );
        }
    }
    
    // Sets avoidance force based on proximity to object fish wants to avoid
    if (bObstacleDetected && !AvoidanceVector.IsNearlyZero())
    {
        AvoidanceVector.Normalize();
        
        float AvoidanceStrength = FMath::Clamp(1.0f - (ClosestHitDistance / RaycastDistance), 0.2f, 1.0f) * 5.0f;
        
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            AvoidanceVector * AvoidanceStrength,
            GetWorld()->GetDeltaSeconds(),
            5.0f 
        );
        
        if (bShowDebugRays)
        {
            // Debus the direction in which the fish lerps towards when avoiding obstacles
            DrawDebugDirectionalArrow(
                GetWorld(),
                CurrentLocation,
                CurrentLocation + CurrentAvoidanceDirection * 50.0f,
                20.0f,
                FColor::Yellow,
                false,
                0.0f,
                0,
                3.0f
            );
        }
    }
    else
    {
        // If no obstacles are detected the avoidance gradually stops affecting movement.
        //Used to smooth out movement
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            FVector::ZeroVector,
            GetWorld()->GetDeltaSeconds(),
            2.0f
        );
    }
}


//Method that handles actual movement
void ACPP_BaseFishAI::MoveAndRotate(float DeltaTime)
{
    //Apply obstacle avoidance if the CurrntAvoidanceDirection is set from CheckObstacles(), else it continiues standard movement
    if (!CurrentAvoidanceDirection.IsNearlyZero())
    {
        //Add avoidance force and interpolated towards the avoidance vector 
        MovementDirection = FMath::VInterpTo(
            MovementDirection,
            CurrentAvoidanceDirection,
            DeltaTime,
            AvoidanceFactor).GetSafeNormal();
        
    }
   
    CurrentVector = MovementDirection * MovementSpeed * MovementSpeedFactor;

    FVector NewLocation = GetActorLocation() + CurrentVector * DeltaTime;
    SetActorLocation(NewLocation);
    
    if (!CurrentVector.IsNearlyZero())
    {
        FRotator NewRotation = CurrentVector.Rotation();
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 5.0f));
    }
}

//NOT IN USE ANYMORE
//Method that was meant to set fish to follow spline
void ACPP_BaseFishAI::FollowSpline(float DeltaTime)
{
    if (!SplineToFollow)
    {
        CurrentBehaviorMode = EFishBehaviorMode::Wander;
        return;
    }
    
    FVector CurrentLocation = GetActorLocation();
    
    float ClosestInputKey = SplineToFollow->FindInputKeyClosestToWorldLocation(CurrentLocation);
    
    FVector ClosestSplinePoint = SplineToFollow->GetLocationAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World);
    
    FVector SplineTangent = SplineToFollow->GetTangentAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World).GetSafeNormal();
    
    FVector SplineNormal = FVector::CrossProduct(SplineTangent, FVector::UpVector).GetSafeNormal();
    FVector Deviation = SplineNormal * FMath::RandRange(-PathDeviation, PathDeviation);
    
    MovementDirection = SplineTangent;
    
}

//Handles random movement and levels fish to parallel with horizon line
void ACPP_BaseFishAI::Wander(float DeltaTime)
{
    MovementDirection = CurrentVector.GetSafeNormal();

    if (((FMath::FRand() * 10.0f) + 15 < TimeSinceDirectionChange || bIsChangingDirection)) 
    {
        TimeSinceDirectionChange = 0.0f;
        DirectionChangeTime += DeltaTime;
        if (!bIsChangingDirection)
        {
            RandomDir = FVector(
             FMath::FRand() * 2.0f - 1.0f,
             FMath::FRand() * 2.0f - 1.0f,
             (FMath::FRand() * 2.0f - 1.0f) * 0.3f
             ).GetSafeNormal(); 
        }
        
        bIsChangingDirection = true;
        MovementDirection = FMath::VInterpTo(
            MovementDirection,
            RandomDir,
            DeltaTime,
            3.0f
        ).GetSafeNormal(); 
    }

    //Levels fish to horizonline if enough time has passed after avoiding an obstacle
    if (TimeSinceObstacleAvoidance > 2.0f && !FMath::IsNearlyZero(CurrentVector.Z))
    {
        FVector ZCorrectedVector = FVector(MovementDirection.X, MovementDirection.Y, 0.0f).GetSafeNormal();
        MovementDirection = FMath::VInterpTo(
           MovementDirection,
           ZCorrectedVector,
           DeltaTime,
           1.0f
       ).GetSafeNormal();
    }

    TimeSinceObstacleAvoidance += DeltaTime;
    TimeSinceDirectionChange += DeltaTime;
    if (DirectionChangeTime > 1.0f)
    {
        bIsChangingDirection = false;
        DirectionChangeTime = 0.0f;
    }
    
}

//Sets MovementDirection towards player character,
//Also slows fish down if it gets to close
//Operates on a timer where after a set time it "looses" interest and goes back to Wander() mode
void ACPP_BaseFishAI::FollowPlayer(float DeltaTime)
{
    MovementDirection = CurrentVector.GetSafeNormal();

    if (bCanFollowPlayer)
    {
        FollowPlayerTimer += DeltaTime;
        MovementSpeedFactor = 2.0f;

        
        APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        if (!PlayerPawn)
        {
            return; 
        }

        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector PlayerDirection = (PlayerLocation - GetActorLocation()).GetSafeNormal();
        
        PlayerDistance = (PlayerLocation - GetActorLocation()).Size();
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Distance: %.2f"), PlayerDistance));
        
        MovementDirection = FMath::VInterpTo(
            MovementDirection,
            PlayerDirection.GetSafeNormal(),
            DeltaTime,
            7.0f
        ).GetSafeNormal();

        if (FollowPlayerTimer > 20.0f)
        {
            MovementSpeedFactor = 1.0f;
            CurrentBehaviorMode = EFishBehaviorMode::Wander;
            FollowPlayerTimer = 0.0f;
        }

        //Sets movementspeed slower based on  proximity to character
        if ((PlayerDistance < 500.0f))
        {
            MovementSpeedFactor = FMath::GetMappedRangeValueClamped(
            FVector2D(200.0f, 500.0f), 
            FVector2D(0.0f, 1.0f),    
            PlayerDistance 
            );
        }
        else
        {
            MovementSpeedFactor = 1.0f;
        }
    }
    else
    {
        //If fish enters follow mode when it should not
        CurrentBehaviorMode = EFishBehaviorMode::Wander;
    }
}

void ACPP_BaseFishAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //The Obstacle avoid runs on a timer set in Begin play. It always gives input to movement if needed (when avoiding obstacles, detecting players)
    //Timer is set to run every 0.1 seconds, can be adjusted, avoidance factor, movementspeed and raycast length may then affect behaviour
    
    switch (CurrentBehaviorMode)
    {
        case EFishBehaviorMode::Wander:
            
            //Handles random movement and makes the fish stay mostly level with the horizon line
            MovementSpeedFactor = 1.0f;
            Wander(DeltaTime);
            
            break;
                
        case EFishBehaviorMode::FollowPlayer:
            
            //Follows player until timer runs out
            FollowPlayer( DeltaTime);

            break;
                
        case EFishBehaviorMode::FollowSpline:

            //This function handles movement on its own
            FollowSpline(DeltaTime);
        
            break;
    }
    
    // Move and rotate the fish, also applies obstacle avoidance
    MoveAndRotate(DeltaTime);

    
}
