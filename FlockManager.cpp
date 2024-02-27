// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockManager.h"

AFlockManager::AFlockManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlockManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeBoids();
}

void AFlockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlockManager::InitializeBoids()
{
	for (int32 i = 0; i < NumBoids; i++)
	{
		// Generate a random position within the specified bounds
		FVector InitialPosition = FMath::RandPointInBox(FBox(WorldBoundsMin, WorldBoundsMax));

		// Generate a random 2D velocity and set the Z component to zero
		FVector InitialVelocity = FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f);

		// Spawn the boid actor
		ABoidActor* NewBoid = GetWorld()->SpawnActor<ABoidActor>(actor, InitialPosition, FRotator::ZeroRotator);
		if (NewBoid)
		{
			// Initialize the boid's properties
			NewBoid->Position = InitialPosition;
			NewBoid->Velocity = InitialVelocity;
            
			// Add the boid to the Boids array
			Boids.Add(NewBoid);
		}
	}
}