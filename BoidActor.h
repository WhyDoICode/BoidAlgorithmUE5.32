#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidActor.generated.h"

UCLASS()
class ZEKE_GAME_API ABoidActor : public AActor
{
	GENERATED_BODY()

public:
	ABoidActor();

	// Properties to represent boid's position and velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	FVector Velocity;

	FVector CalculateCohesionMoveTowardsAnchor(const TArray<AActor*>& NearbyBoids, const FVector& AnchorPosition);
	FVector SteerTowardsAnchor(const FVector& AnchorPosition);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	FVector WorldBoundsMin = FVector(-1500, -1500, 0); // Min world bounds for initial boid position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	FVector WorldBoundsMax = FVector(1500, 1500, 0);  // Max world bounds for initial boid position

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	float Margin = 300.0f; // Distance from the edge within which the boid starts turning back

	// Boid settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float VisualRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float MinDistance; // For separation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float SpeedLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float CenteringFactor; // For cohesion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float AvoidFactor; // For separation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float MatchingFactor; // For alignment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	  float TurnFactor; // For keeping within bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory")
	float MaxForce; // For cohesion

	// Update methods for boid behavior
	void FlyTowardsCenter(const TArray<ABoidActor*>& NearbyBoids);
	void AvoidOthers(const TArray<ABoidActor*>& NearbyBoids);
	void MatchVelocity(const TArray<ABoidActor*>& NearbyBoids);
	void LimitSpeed();
	void KeepWithinBounds();


	TArray<ABoidActor*> GetNearbyBoids();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
