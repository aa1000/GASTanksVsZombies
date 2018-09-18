// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Components/SphereComponent.h"
#include "PawnWithAbilities.h"
// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Range Sphere"));
	//SpawnSphere->SetGenerateOverlapEvents(false);
	SpawnSphere->SetCollisionProfileName("NoCollision");

	SpawnTimer = 5.f;
	SpawnCollisionProfile = TEXT("Spawner");
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnEnemy, SpawnTimer, true);
}

void ASpawner::SpawnEnemy()
{
	//UE_LOG(LogTemp, Warning, TEXT("Spawn enemy called by timer"));
	if(EnemyToSpawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy Class Valid"));
		if(UWorld* World = GetWorld())
		{
			FCollisionShape CollisionShape;
			CollisionShape.SetCapsule(SpawnSphere->GetCollisionShape().GetSphereRadius(), 200);
			if( !World->OverlapAnyTestByProfile(GetActorLocation(), FQuat::Identity,
				SpawnCollisionProfile, CollisionShape) )
			{
				//UE_LOG(LogTemp, Warning, TEXT("Can Spawn Zombie"));
				FVector Loc;
				FRotator Rot;
				RandomizeSpawnLocationRotation(Loc, Rot);

				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				World->SpawnActor<APawnWithAbilities>(EnemyToSpawn, Loc, Rot, SpawnInfo);
					//UE_LOG(LogTemp, Warning, TEXT("Zombie Spawned"));
			}
		}
	}
}

void ASpawner::RandomizeSpawnLocationRotation(FVector& Loc, FRotator& Rot)
{
	Loc = GetActorLocation();
	Rot = GetActorRotation();

	const float Range = SpawnSphere->GetCollisionShape().GetSphereRadius();
	Loc.X += FMath::RandRange(0.f, Range);
	Loc.Y += FMath::RandRange(0.f, Range);

	Rot.Yaw = FMath::RandRange(0.f, 360.f);
}

