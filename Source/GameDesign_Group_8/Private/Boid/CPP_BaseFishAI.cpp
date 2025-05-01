// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SplineComponent.h"
#include "Boid/CPP_BaseFishAI.h"


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

void ACPP_BaseFishAI::CheckObstacles()
{
    const float RaycastDistance = 150.0f * (MovementSpeed / 100.0f) + 100; // Scale raycast distance with speed + an initial offset
    FVector CurrentLocation = GetActorLocation();
    FVector CurrentDirection = CurrentVector.GetSafeNormal();
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); 
    
    // Define ray directions (forward + 4 angled rays)
    TArray<FVector> Directions;
    Directions.Add(CurrentDirection);
    
    // Add angled rays (forward-left, forward-right, forward-up, forward-down)
    FVector RightVector = FVector::CrossProduct(CurrentDirection, FVector::UpVector).GetSafeNormal();
    FVector UpVector = FVector::CrossProduct(RightVector, CurrentDirection).GetSafeNormal();

    //Edit VisionConeMultiplier to adjust max angle of raycasts
    Directions.Add((CurrentDirection + RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection + UpVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - UpVector * VisionConeMultiplier).GetSafeNormal());
    
    // Perform raycasts
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
            float Weight = 1.0f - (Hit.Distance / RaycastDistance);
            AvoidanceVector += Hit.Normal * Weight;
        
            if (Hit.Distance < ClosestHitDistance)
            {
                ClosestHitDistance = Hit.Distance;
            }
        
            // Debug visualization - increased duration and thickness
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
            // Draw green rays for no hit
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
    
    // Calculate avoidance force
    if (bObstacleDetected && !AvoidanceVector.IsNearlyZero())
    {
        AvoidanceVector.Normalize();
        
        // Stronger avoidance as obstacles get closer
        float AvoidanceStrength = FMath::Clamp(1.0f - (ClosestHitDistance / RaycastDistance), 0.2f, 1.0f) * 5.0f;
        
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            AvoidanceVector * AvoidanceStrength,
            GetWorld()->GetDeltaSeconds(),
            5.0f 
        );
        
        if (bShowDebugRays)
        {
            // Visualize the avoidance direction
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
        // Gradually decays avoidance force when no obstacles are detected
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            FVector::ZeroVector,
            GetWorld()->GetDeltaSeconds(),
            2.0f
        );
    }
}

void ACPP_BaseFishAI::MoveAndRotate(float DeltaTime)
{
    // Apply obstacle avoidance as a steering force
    if (!CurrentAvoidanceDirection.IsNearlyZero())
    {
        // Add avoidance force
        MovementDirection = (MovementDirection + CurrentAvoidanceDirection * AvoidanceFactor).GetSafeNormal();
        
        // Debug visualization of resulting direction
        if (bShowDebugRays)
        {
            DrawDebugDirectionalArrow(
                GetWorld(),
                GetActorLocation(),
                GetActorLocation() + MovementDirection * 100.0f,
                20.0f,
                FColor::Blue,
                false,
                0.0f,
                0,
                3.0f
            );
        }
    }
   
    //Set new movement vector
    CurrentVector = MovementDirection * MovementSpeed * MovementSpeedFactor;

    // Apply movement
    FVector NewLocation = GetActorLocation() + CurrentVector * DeltaTime;
    SetActorLocation(NewLocation);
    
    // Update rotation to face movement direction
    if (!CurrentVector.IsNearlyZero())
    {
        FRotator NewRotation = CurrentVector.Rotation();
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 5.0f));
    }
}

void ACPP_BaseFishAI::FollowSpline(float DeltaTime)
{
    if (!SplineToFollow)
    {
        CurrentBehaviorMode = EFishBehaviorMode::Wander;
        return;
    }
    
    FVector CurrentLocation = GetActorLocation();
    
    // Find the closest point on the spline to the fish's current location
    float ClosestInputKey = SplineToFollow->FindInputKeyClosestToWorldLocation(CurrentLocation);
    
    FVector ClosestSplinePoint = SplineToFollow->GetLocationAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World);
    
    FVector SplineTangent = SplineToFollow->GetTangentAtSplineInputKey(ClosestInputKey, ESplineCoordinateSpace::World).GetSafeNormal();
    
    FVector SplineNormal = FVector::CrossProduct(SplineTangent, FVector::UpVector).GetSafeNormal();
    FVector Deviation = SplineNormal * FMath::RandRange(-PathDeviation, PathDeviation);
    
    // Calculate movement direction based on spline tangent
    MovementDirection = SplineTangent;
    
    // Optional: Debug visualization
#if WITH_EDITOR
    if (GetWorld()->IsPlayInEditor() && bShowDebugRays)
    {
        // Draw line from fish to closest spline point
        DrawDebugLine(GetWorld(), CurrentLocation, ClosestSplinePoint, FColor::White, false, -1.0f, 0, 2.0f);
        
        // Draw spline tangent
        DrawDebugLine(GetWorld(), ClosestSplinePoint, ClosestSplinePoint + SplineTangent * 100.0f, FColor::Red, false, -1.0f, 0, 2.0f);
        
        // Draw spline normal
        DrawDebugLine(GetWorld(), ClosestSplinePoint, ClosestSplinePoint + SplineNormal * 100.0f, FColor::Green, false, -1.0f, 0, 2.0f);
    }
#endif
    
}


void ACPP_BaseFishAI::Wander(float DeltaTime)
{
    MovementDirection = CurrentVector.GetSafeNormal();

    //Change direction if enough time has passed, with a slight randomization to waiting time
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

    //Levels fish to horizonline
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

    // Update time counters
    TimeSinceObstacleAvoidance += DeltaTime;
    TimeSinceDirectionChange += DeltaTime;
    if (DirectionChangeTime > 1.0f)
    {
        bIsChangingDirection = false;
        DirectionChangeTime = 0.0f;
    }
    
}


void ACPP_BaseFishAI::FollowPlayer(float DeltaTime)
{
    // Store current direction before applying forces
    MovementDirection = CurrentVector.GetSafeNormal();

    // Follow player if true
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

        // If followtimer runs out stop following
        if (FollowPlayerTimer > 20.0f)
        {
            MovementSpeedFactor = 1.0f;
            CurrentBehaviorMode = EFishBehaviorMode::Wander;
            FollowPlayerTimer = 0.0f;
        }

        //Slows fish down when approaching player
        if ((PlayerDistance < 500.0f))
        {
            MovementSpeedFactor = FMath::GetMappedRangeValueClamped(
            FVector2D(200.0f, 500.0f), 
            FVector2D(0.0f, 1.0f),    
            PlayerDistance 
            );
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
