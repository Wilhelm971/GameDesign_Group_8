#include "Boid/CPP_BoidActor.h"
#include "DrawDebugHelpers.h"
#include "VectorUtil.h"
#include "Kismet/KismetMathLibrary.h"

ACPP_BoidActor::ACPP_BoidActor()
{
    PrimaryActorTick.bCanEverTick = true;
    
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    FishMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishMesh"));
    FishMesh->SetupAttachment(RootComponent);
    
    FishMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    FishMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    FishMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    
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
}

void ACPP_BoidActor::BeginPlay()
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
         &ACPP_BoidActor::CheckObstacles,
         0.1f,
         true,
         RandomDelay
     );
}

void ACPP_BoidActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    Super::EndPlay(EndPlayReason);
}

void ACPP_BoidActor::SetSplineToFollow(const AActor* NewSplineActor)
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

void ACPP_BoidActor::CheckObstacles()
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

void ACPP_BoidActor::MoveAndRotate(float DeltaTime)
{
    // Store current direction before applying forces
    FVector MovementDirection = CurrentVector.GetSafeNormal();
    
    // Apply obstacle avoidance as a steering force
    if (!CurrentAvoidanceDirection.IsNearlyZero())
    {
        // Add avoidance force
        MovementDirection = (MovementDirection + CurrentAvoidanceDirection * AvoidanceFactor * DeltaTime).GetSafeNormal();
        
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

void ACPP_BoidActor::FollowSpline(float DeltaTime)
{
    // Check if we have a valid spline
    if (!SplineToFollow)
    {
        // No spline, revert to wandering
        CurrentBehaviorMode = EFishBehaviorMode::Wander;
        return;
    }
    
    // Get spline length
    float SplineLength = SplineToFollow->GetSplineLength();
    
    // Apply speed variation for more natural movement
    float CurrentSpeed = MovementSpeed * (1.0f + FMath::RandRange(-SpeedVariation, SpeedVariation));
    
    // Calculate new distance along spline
    CurrentSplineDistance += SplineDirection * CurrentSpeed * DeltaTime;
    
    // Handle reaching the end of the spline
    if (CurrentSplineDistance >= SplineLength)
    {
        if (bLoopAlongSpline)
        {
            // Loop back to start
            CurrentSplineDistance = FMath::Fmod(CurrentSplineDistance, SplineLength);
        }
        else
        {
            // Reverse direction
            CurrentSplineDistance = SplineLength;
            SplineDirection = -1;
        }
    }
    else if (CurrentSplineDistance <= 0.0f && !bLoopAlongSpline)
    {
        // Reached the start while going backward, reverse direction
        CurrentSplineDistance = 0.0f;
        SplineDirection = 1;
    }
    
    // Get position and tangent from spline
    FVector SplinePosition = SplineToFollow->GetLocationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World);
    FVector SplineTangent = SplineToFollow->GetTangentAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World).GetSafeNormal();
    
    // Update deviation for natural movement
    DeviationChangeTimer -= DeltaTime;
    if (DeviationChangeTimer <= 0.0f)
    {
        // Create a new random deviation perpendicular to the spline direction
        FVector Up = FVector(0, 0, 1);
        FVector Right = FVector::CrossProduct(SplineTangent, Up).GetSafeNormal();
        FVector DeviationDirection = Right * FMath::RandRange(-1.0f, 1.0f) + Up * FMath::RandRange(-1.0f, 1.0f);
        DeviationDirection.Normalize();
        
        // Apply deviation scale
        CurrentDeviation = DeviationDirection * FMath::RandRange(0.0f, PathDeviation);
        
        // Reset timer
        DeviationChangeTimer = FMath::RandRange(1.0f, 3.0f);
    }
    
    // Apply deviation to target position
    FVector TargetPosition = SplinePosition + CurrentDeviation;
    
    
    // Apply obstacle avoidance as a steering force
    if (!CurrentAvoidanceDirection.IsNearlyZero())
    {
        // Add avoidance force
        TargetPosition = (TargetPosition + CurrentAvoidanceDirection * AvoidanceFactor * DeltaTime).GetSafeNormal();
        
        // Debug visualization of resulting direction
        if (bShowDebugRays)
        {
            DrawDebugDirectionalArrow(
                GetWorld(),
                GetActorLocation(),
                GetActorLocation() + TargetPosition * 100.0f,
                20.0f,
                FColor::Blue,
                false,
                0.0f,
                0,
                3.0f
            );
        }
    }
    // Calculate direction to target
    FVector CurrentLocation = GetActorLocation();
    FVector DirectionToTarget = (TargetPosition - CurrentLocation).GetSafeNormal();
    
    // Calculate new rotation (blend between spline tangent and direction to target)
    FRotator TargetRotation = FMath::Lerp(DirectionToTarget.Rotation(), SplineTangent.Rotation(), 0.5f);
    FRotator CurrentRotation = GetActorRotation();
    
    // Use VInterpTo for smooth rotation towards target
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
    
    // Update rotation
    SetActorRotation(NewRotation);
    // Apply movement
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetPosition, DeltaTime, 2.0f);
    
    // Set new location
    SetActorLocation(NewLocation);
    
  
    // Debug visualization
    #if WITH_EDITOR
    if (GetWorld()->IsPlayInEditor())
    {
        // Draw the spline path
        for (float Dist = 0; Dist < SplineLength; Dist += SplineLength / 50.0f)
        {
            DrawDebugPoint(GetWorld(), SplineToFollow->GetLocationAtDistanceAlongSpline(Dist, ESplineCoordinateSpace::World), 5.0f, FColor::Blue, false, -1, 0);
        }
        
        // Draw current target point
        DrawDebugSphere(GetWorld(), TargetPosition, 15.0f, 8, FColor::Red, false, -1, 0, 1.0f);
        
        // Draw line from current position to target
        DrawDebugLine(GetWorld(), CurrentLocation, TargetPosition, FColor::Green, false, -1, 0, 2.0f);
    }
    #endif
}

void ACPP_BoidActor::Wander(float DeltaTime)
{
    FVector MovementDirection = CurrentVector.GetSafeNormal();

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


void ACPP_BoidActor::FollowPlayer(float DeltaTime)
{
    // Store current direction before applying forces
    FVector MovementDirection = CurrentVector.GetSafeNormal();

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

void ACPP_BoidActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //The Obstacle avoid runs on a timer set in Begin play. It always gives input to movement if needed (when avoiding obstacles, detecting players)
    //Timer is set to run every 0.1 seconds, can be adjusted, avoidance factor, movementspeed and raycast length may then affect behaviour
    
    switch (CurrentBehaviorMode)
    {
        case EFishBehaviorMode::Wander:
            //Handles random movement and makes the fish stay mostly level with the horizon line
            Wander(DeltaTime);
            
            // Move and rotate the fish, also applies obstacle avoidance
            MoveAndRotate(DeltaTime);
            break;
                
        case EFishBehaviorMode::FollowPlayer:
            //Follows player until timer runs out
            FollowPlayer( DeltaTime);

            // Move and rotate the fish, also applies obstacle avoidance
            MoveAndRotate(DeltaTime);
            break;
                
        case EFishBehaviorMode::FollowSpline:

            //This function handles movement on its own
            FollowSpline(DeltaTime);
            break;
    }
    
}


