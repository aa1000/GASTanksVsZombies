// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class TANKSVSZOMBIES_API ASpawner : public AActor
{
	GENERATED_BODY()
	
	FTimerHandle SpawnTimerHandle;

public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:

	UPROPERTY(VisibleAnywhere, Category = Spawner)
	class USphereComponent* SpawnSphere;

	// the cooldown between spawning enemies
	UPROPERTY(EditAnywhere, Category = Spawner)
	float SpawnTimer;

	// The type of enemy to spawn
	UPROPERTY(EditAnywhere, Category = Spawner)
	TSubclassOf<class APawnWithAbilities> EnemyToSpawn;

	UPROPERTY(EditAnywhere, Category = Spawner)
	FName SpawnCollisionProfile;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnEnemy();
	
	void RandomizeSpawnLocationRotation(FVector& Loc, FRotator& Rot);
	
};
