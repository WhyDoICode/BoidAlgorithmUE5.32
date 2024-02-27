// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidActor.h"
#include "GameFramework/Actor.h"
#include "FlockManager.generated.h"

UCLASS()
class ZEKE_GAME_API AFlockManager : public AActor
{
	GENERATED_BODY()

public:
	AFlockManager();
	void InitializeBoids();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	TArray<ABoidActor*> Boids;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	TSubclassOf<ABoidActor> actor;

	// Constants for the number of boids and the world bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	 int32 NumBoids = 100; // Number of boids to create
	 FVector WorldBoundsMin = FVector(-1500, -1500, 0); // Min world bounds for initial boid position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	 FVector WorldBoundsMax = FVector(1500, 1500, 0);  // Max world bounds for initial boid position

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
