// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TANKSVSZOMBIES_API AProjectile : public AActor
{
	GENERATED_BODY()

public:

	// How fast the projectile flies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile")
	float ProjectileSpeed;

	// How much damage the projectile causes to hit actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile")
	float ProjectileDamage;

	// The projectile radius for collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile")
	float ExplosionRaduis;

	// Describes what this projectile hits
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile")
	FName MovementCollisionProfile;

	// Name of the tag that the gameplay event waiting for hit has
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Projectile")
	FGameplayTag GameplayEventHitTag;

	// Sets default values for this actor's properties
	AProjectile();

protected:
	//	Sprite for the projectile body
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	//class UPaperSpriteComponent* ProjectileSprite;

	//	Collision volume for the projectile body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UBoxComponent* ProjectileBody;

	//	Projectile movement for this projectile
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	//class UProjectileMovementComponent* ProjectileMovement;

	FTimerHandle ExplodeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Projectile")
	float TimeToExplode;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void HandleHitActor(AActor* HitActor);

	virtual void Explode();

	// the base version of this function just destroys the projectile
	UFUNCTION(BlueprintNativeEvent, Category = Projectile)
	void OnExplode();

	virtual void MoveProjectile(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
