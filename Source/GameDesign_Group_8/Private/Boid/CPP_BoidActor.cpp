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
    bShouldFollowPlayer = false;
    MovementSpeedFactor = 1.0f;
}

void ACPP_BoidActor::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentVector = GetActorForwardVector().GetSafeNormal() * MovementSpeed;

    float RandomDelay = FMath::FRandRange(0.0f, 0.1f);
    
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

void ACPP_BoidActor::CheckObstacles()
{
    const float RaycastDistance = 150.0f * (MovementSpeed / 100.0f) + 100; // Scale raycast distance with speed
    FVector CurrentLocation = GetActorLocation();
    FVector CurrentDirection = CurrentVector.GetSafeNormal();
    
    // Setup collision query
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    // Define ray directions (forward + 4 angled rays)
    TArray<FVector> Directions;
    Directions.Add(CurrentDirection);
    
    // Add angled rays (forward-left, forward-right, forward-up, forward-down)
    FVector RightVector = FVector::CrossProduct(CurrentDirection, FVector::UpVector).GetSafeNormal();
    FVector UpVector = FVector::CrossProduct(RightVector, CurrentDirection).GetSafeNormal();
    
    Directions.Add((CurrentDirection + RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - RightVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection + UpVector * VisionConeMultiplier).GetSafeNormal());
    Directions.Add((CurrentDirection - UpVector * VisionConeMultiplier).GetSafeNormal());
    
    // Perform raycasts
    FVector AvoidanceVector = FVector::ZeroVector;
    float ClosestHitDistance = RaycastDistance;
    bool bObstacleDetected = false;
    
    // Debug: Output to confirm this method is being called
    UE_LOG(LogTemp, Display, TEXT("CheckObstacles running, bShowDebugRays = %s"), 
           bShowDebugRays ? TEXT("true") : TEXT("false"));
    
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

        if (Hit.GetActor() && Hit.GetActor()->IsA(APawn::StaticClass()))
        {
            bShouldFollowPlayer = true;
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
                    false,       // persistent lines
                    0.0f,        // lifetime (0 = single frame)
                    0,           // depth priority
                    3.0f         // increased thickness
                );
                
                // Debug sphere at hit point for better visibility
                DrawDebugSphere(
                    GetWorld(),
                    Hit.Location,
                    5.0f,        // radius
                    8,           // segments
                    FColor::Red,
                    false,
                    0.0f
                );
                
                // Log hit
                UE_LOG(LogTemp, Display, TEXT("Hit detected! Distance: %.2f"), Hit.Distance);
            }
        }
        else if (bShowDebugRays)
        {
            // Draw green rays for no hit - increased thickness
            DrawDebugLine(
                GetWorld(),
                CurrentLocation,
                CurrentLocation + Dir * RaycastDistance,
                FColor::Green,
                false, 
                0.0f,      // lifetime (0 = single frame)
                0, 
                3.0f       // increased thickness
            );
        }
    }
    
    // Calculate avoidance force
    if (bObstacleDetected && !AvoidanceVector.IsNearlyZero())
    {
        AvoidanceVector.Normalize();
        
        // Stronger avoidance as obstacles get closer
        float AvoidanceStrength = FMath::Clamp(1.0f - (ClosestHitDistance / RaycastDistance), 0.2f, 1.0f) * 5.0f;
        
        // Smoothly update avoidance direction - faster response time
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            AvoidanceVector * AvoidanceStrength,
            GetWorld()->GetDeltaSeconds(),
            5.0f  // Increased from 3.0 to 5.0 for faster response
        );
        
        if (bShowDebugRays)
        {
            // Visualize the avoidance direction
            DrawDebugDirectionalArrow(
                GetWorld(),
                CurrentLocation,
                CurrentLocation + CurrentAvoidanceDirection * 50.0f,
                20.0f,  // arrow size
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
        // Gradually decay avoidance when no obstacles
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

    // Follow player if true
    if (bShouldFollowPlayer && bCanFollowPlayer)
    {
        FollowPlayerTimer += DeltaTime;
        MovementSpeedFactor = 2.0f;

        
        APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        if (!PlayerPawn)
        {
            return; // Exit the function as we can't follow the player.
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
            bShouldFollowPlayer = false;
            FollowPlayerTimer = 0.0f;
        }
    }
    
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
    
    //Change direction if enough time has passed, with a slight randomization to waiting time
    if (((FMath::FRand() * 10.0f) + 15 < TimeSinceDirectionChange || bIsChangingDirection) && !bShouldFollowPlayer) 
    {
        TimeSinceDirectionChange = 0.0f;
        DirectionChangeTime += DeltaTime;
        if (!bIsChangingDirection)
        {
            RandomDir = FVector(
             FMath::FRand() * 2.0f - 1.0f,
             FMath::FRand() * 2.0f - 1.0f,
             (FMath::FRand() * 2.0f - 1.0f) * 0.3f // Less vertical movement
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
     
    //Sets movement
    if ((PlayerDistance < 500.0f) && bShouldFollowPlayer)
    {
        MovementSpeedFactor = FMath::GetMappedRangeValueClamped(
        FVector2D(200.0f, 500.0f), // Input range: distance thresholds (clamped between 200 and 500)
        FVector2D(0.0f, 1.0f),    // Output range: 0.0 speed factor (stop) at 200, full speed (1.0) at 500
        PlayerDistance // Player's current distance evaluated within the range
    );

    }
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

//----NOT IN USE----
void ACPP_BoidActor::KeepInBounds()
{
    // Only apply if boundary volume is set
    if (!BoundaryVolume)
        return;
        
    // Get boundary box extent and center
    FVector BoxOrigin, BoxExtent;
    BoundaryVolume->GetActorBounds(false, BoxOrigin, BoxExtent);
    
    // Calculate min and max bounds from the volume
    FVector BoundsMin = BoxOrigin - BoxExtent;
    FVector BoundsMax = BoxOrigin + BoxExtent;
    
    FVector CurrentLocation = GetActorLocation();
    FVector TowardCenter = BoxOrigin - CurrentLocation;
    bool bNeedsCorrection = false;
    
    // Check if fish is approaching environment boundaries
    
    // X-axis (forward/backward)
    if (CurrentLocation.X < BoundsMin.X + BoundaryMargin && CurrentVector.X < 0)
    {
        // Instead of just flipping direction, steer toward center
        CurrentVector.X += TowardCenter.X * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    else if (CurrentLocation.X > BoundsMax.X - BoundaryMargin && CurrentVector.X > 0)
    {
        CurrentVector.X += TowardCenter.X * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    
    // Y-axis (left/right)
    if (CurrentLocation.Y < BoundsMin.Y + BoundaryMargin && CurrentVector.Y < 0)
    {
        CurrentVector.Y += TowardCenter.Y * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    else if (CurrentLocation.Y > BoundsMax.Y - BoundaryMargin && CurrentVector.Y > 0)
    {
        CurrentVector.Y += TowardCenter.Y * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    
    // Z-axis (up/down)
    if (CurrentLocation.Z < BoundsMin.Z + BoundaryMargin && CurrentVector.Z < 0)
    {
        CurrentVector.Z += TowardCenter.Z * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    else if (CurrentLocation.Z > BoundsMax.Z - BoundaryMargin && CurrentVector.Z > 0)
    {
        CurrentVector.Z += TowardCenter.Z * BoundaryAvoidanceStrength * GetWorld()->GetDeltaSeconds();
        bNeedsCorrection = true;
    }
    
    // Normalize if we made changes
    if (bNeedsCorrection)
    {
        CurrentVector.Normalize();
        CurrentVector *= MovementSpeed;
    }
}

void ACPP_BoidActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Check for obstacles
    //VOID METHOD IS CALLED IN TIMER FROM BEGINPLAY
    //CheckObstacles();
    
    // Keep within environment bounds if set
    //KeepInBounds();
    
    // Move and rotate the fish
    MoveAndRotate(DeltaTime);

    // Update time counters
    TimeSinceObstacleAvoidance += DeltaTime;
    TimeSinceDirectionChange += DeltaTime;
    if (DirectionChangeTime > 1.0f)
    {
        bIsChangingDirection = false;
        DirectionChangeTime = 0.0f;
    }
}