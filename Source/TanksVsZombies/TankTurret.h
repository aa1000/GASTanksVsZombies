// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankTurret.generated.h"

UCLASS()
class TANKSVSZOMBIES_API ATankTurret : public AActor
{
	GENERATED_BODY()


	class ATank* OwningTank;
	APlayerController* OwningTankPC;
	
	uint32 bIsFiring : 1;

public:	
	// Sets default values for this actor's properties
	ATankTurret();

protected:
	// Shows the turret direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Direction)
	class UArrowComponent* TurretDirection;

	//sprite for the turret body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprites)
	class UPaperSpriteComponent* TurretBody;

	// The type of pojectile this turret can fire on fire 1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<class AProjectile> Fire1ProjectileClass;

	// The type of pojectile this turret can fire on fire 2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<class AProjectile> Fire2ProjectileClass;

	// Time to reload another shot after firing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float ReloadTime;

	FTimerHandle FiringTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MoveTurret(float DeltaTime);

	UFUNCTION()
	virtual void FireProjectile(TSubclassOf<class AProjectile> ProjectileToFire);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartFiring(uint8 WeaponToFire);
	void StopFiring();
	
};
