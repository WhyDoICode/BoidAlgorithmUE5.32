#include "BoidActor.h"

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABoidActor::ABoidActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize Position and Velocity
    Position = GetActorLocation();
    Velocity = FVector(FMath::RandRange(-5.0f, 5.0f), FMath::RandRange(-5.0f, 5.0f), 0.0f);
}

void ABoidActor::BeginPlay( )
{
    Super::BeginPlay( );
}

void ABoidActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    TArray<ABoidActor*> NearbyBoids = GetNearbyBoids( );

    // Boid Behaviors
    FlyTowardsCenter( NearbyBoids );
    AvoidOthers( NearbyBoids );
    MatchVelocity( NearbyBoids );
    LimitSpeed( );
    KeepWithinBounds( );

    //   <CODE TO HELP BOIDS MOVE TOWARDS A CHARACTER>
    // Assuming PlayerCharacter is the anchor for steering

    //FVector AnchorPosition = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
    //FVector AnchorMove = SteerTowardsAnchor(AnchorPosition) * TurnFactor; // Make sure TurnFactor is properly defined and adjusted
    //Velocity += AnchorMove;

    FVector NewPosition = GetActorLocation( ) + Velocity * DeltaTime;
    SetActorLocation( NewPosition );
}



FVector ABoidActor::CalculateCohesionMoveTowardsAnchor(const TArray<AActor*>& NearbyBoids, const FVector& AnchorPosition)
{
    FVector CenterMass = FVector::ZeroVector;
    int Count = 0;

    for (AActor* Boid : NearbyBoids)
    {
        CenterMass += Boid->GetActorLocation();
        Count++;
    }

    if (Count > 0)
    {
        CenterMass /= Count;
        // Move a percentage of the way towards the anchor as well
        CenterMass = CenterMass + (AnchorPosition - CenterMass) * CenteringFactor;
    }

    return (CenterMass - GetActorLocation()).GetClampedToMaxSize(MaxForce);
}

FVector ABoidActor::SteerTowardsAnchor(const FVector& AnchorPosition)
{
    FVector DesiredDirection = (AnchorPosition - GetActorLocation()).GetSafeNormal() * SpeedLimit;
    return (DesiredDirection - Velocity).GetClampedToMaxSize(MaxForce);
}

void ABoidActor::FlyTowardsCenter(const TArray<ABoidActor*>& NearbyBoids)
{
    FVector Center = FVector::ZeroVector;
    int NumNeighbors = 0;

    for (ABoidActor* OtherBoid : NearbyBoids)
    {
        if (OtherBoid != this)
        {
            Center += OtherBoid->GetActorLocation();
            NumNeighbors++;
        }
    }

    if (NumNeighbors > 0)
    {
        Center /= NumNeighbors;
        FVector TowardsCenter = Center - GetActorLocation();
        Velocity += TowardsCenter * CenteringFactor; 
    }
}

void ABoidActor::AvoidOthers(const TArray<ABoidActor*>& NearbyBoids)
{
    FVector MoveAway = FVector::ZeroVector;
    for (ABoidActor* OtherBoid : NearbyBoids)
    {
        if (OtherBoid != this)
        {
            float Distance = FVector::Dist(GetActorLocation(), OtherBoid->GetActorLocation());
            if (Distance < MinDistance) // MinDistance should be defined in your class
            {
                MoveAway += GetActorLocation() - OtherBoid->GetActorLocation();
            }
        }
    }

    Velocity += MoveAway * AvoidFactor; 
}

void ABoidActor::MatchVelocity(const TArray<ABoidActor*>& NearbyBoids)
{
    FVector AverageVelocity = FVector::ZeroVector;
    int NumNeighbors = 0;

    for (ABoidActor* OtherBoid : NearbyBoids)
    {
        if (OtherBoid != this)
        {
            AverageVelocity += OtherBoid->Velocity;
            NumNeighbors++;
        }
    }

    if (NumNeighbors > 0)
    {
        AverageVelocity /= NumNeighbors;
        Velocity += (AverageVelocity - Velocity) * MatchingFactor; 
    }
}

void ABoidActor::LimitSpeed()
{
    // SpeedLimit should be defined in your class
    if (Velocity.Size() > SpeedLimit) 
    {
        Velocity = Velocity.GetSafeNormal() * SpeedLimit;
    }
}

void ABoidActor::KeepWithinBounds()
{

    FVector CurrentPosition = GetActorLocation();
    FVector NewVelocity = Velocity;

    // Adjust for X bounds
    if (CurrentPosition.X < WorldBoundsMin.X + Margin)
    {
        NewVelocity.X += TurnFactor * ((WorldBoundsMin.X + Margin) - CurrentPosition.X) / Margin;
    }
    else if (CurrentPosition.X > WorldBoundsMax.X - Margin)
    {
        NewVelocity.X -= TurnFactor * (CurrentPosition.X - (WorldBoundsMax.X - Margin)) / Margin;
    }

    // Adjust for Y bounds
    if (CurrentPosition.Y < WorldBoundsMin.Y + Margin)
    {
        NewVelocity.Y += TurnFactor * ((WorldBoundsMin.Y + Margin) - CurrentPosition.Y) / Margin;
    }
    else if (CurrentPosition.Y > WorldBoundsMax.Y - Margin)
    {
        NewVelocity.Y -= TurnFactor * (CurrentPosition.Y - (WorldBoundsMax.Y - Margin)) / Margin;
    }

    // Update the velocity, ensuring it doesn't exceed the speed limit
    Velocity = NewVelocity.GetClampedToMaxSize(SpeedLimit);
}


TArray<ABoidActor*> ABoidActor::GetNearbyBoids()
{
    // Get all nearby boids within the visual range

    //This is really simple implementation, use Spatial partioning to improve this further.
    TArray<ABoidActor*> nearbyBoids;
    for (TActorIterator<ABoidActor> It(GetWorld()); It; ++It)
    {
        ABoidActor* other = *It;
        if (other != this && FVector::Dist(this->GetActorLocation(), other->GetActorLocation()) <= VisualRange)
        {
            nearbyBoids.Add(other);
        }
    }
    return nearbyBoids;
}

