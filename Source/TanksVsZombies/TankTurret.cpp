// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TVZStatics.h"
#include "TimerManager.h"
#include "Projectile.h"

// Sets default values
ATankTurret::ATankTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	RootComponent = TurretDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Turret Direction Arrow"));
	TurretBody = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Turret Body"));

	//TurretDirection->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	TurretBody->AttachToComponent(TurretDirection, FAttachmentTransformRules::KeepWorldTransform);

	ReloadTime = 1.f;
}

// Called when the game starts or when spawned
void ATankTurret::BeginPlay()
{
	Super::BeginPlay();

	OwningTank = Cast<ATank>(GetParentActor());
	if(OwningTank)
		OwningTankPC = Cast<APlayerController>(OwningTank->GetController());

}

void ATankTurret::MoveTurret(float DeltaTime)
{
	check(TurretDirection);
	FVector2D AimLocation;

	if(OwningTankPC->GetMousePosition(AimLocation.X, AimLocation.Y) && OwningTank->GetHealth() > 0)
	{
		FVector2D TurretLocation = FVector2D::ZeroVector;
		UGameplayStatics::ProjectWorldToScreen(OwningTankPC, TurretDirection->GetComponentLocation(), TurretLocation);

		float DesiredYaw;
		if(UTVZStatics::FindLookAtAngle2D(TurretLocation, AimLocation, DesiredYaw))
		{
			DesiredYaw += 90;
			FRotator CurrentRotation = TurretDirection->GetComponentRotation();
			float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, DesiredYaw);

			float MaxDeltaYawThisFrame = OwningTank->GetTurretYawSpeed() * DeltaTime;

			// preform the current frame rotation
			if(MaxDeltaYawThisFrame >= FMath::Abs(DeltaYaw))
				// we can get there on this frame just set it
				CurrentRotation.Yaw = DesiredYaw; 
			else 
				// we can't turn as far as we want so just turn as far as we can
				CurrentRotation.Yaw += FMath::Sign(DeltaYaw) * MaxDeltaYawThisFrame;

			TurretDirection->SetWorldRotation(CurrentRotation);
		}

	}
}


void ATankTurret::FireProjectile(TSubclassOf<AProjectile> ProjectileToFire)
{
	UWorld* World = GetWorld();
	if(World && OwningTank->CanAttack() && ProjectileToFire && bIsFiring)
	{
		FVector Loc = TurretBody->GetSocketLocation("Muzzle");
		FRotator Rot = TurretBody->GetComponentRotation();


		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = OwningTank;
		SpawnInfo.Instigator = Instigator;
		AProjectile* NewProj = World->SpawnActor<AProjectile>(ProjectileToFire, Loc, Rot, SpawnInfo);
	}
	else
		// restet the timer if we are not holding the firing key
		GetWorldTimerManager().ClearTimer(FiringTimerHandle);
}

// Called every frame
void ATankTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(OwningTank && OwningTankPC)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OwningTank And OwningTankPC Valid"));
		// Handle Movement
		MoveTurret(DeltaTime);

	}
}

void ATankTurret::StartFiring(uint8 WeaponToFire)
{
	if(GetWorldTimerManager().IsTimerActive(FiringTimerHandle))
		return;

	bIsFiring = true;
	if(WeaponToFire == 1)
	{
		FireProjectile(Fire1ProjectileClass);
		// Set the timer if we are holding the firing 1 key
		FTimerDelegate ReloadTimerDelegate = FTimerDelegate::CreateUObject( this, &ATankTurret::FireProjectile, Fire1ProjectileClass );
		GetWorldTimerManager().SetTimer(FiringTimerHandle, ReloadTimerDelegate, ReloadTime, true);
	}
	else if(WeaponToFire == 2)
	{
		FireProjectile(Fire2ProjectileClass);
		// Set the timer if we are holding the firing 2 key
		FTimerDelegate ReloadTimerDelegate = FTimerDelegate::CreateUObject( this, &ATankTurret::FireProjectile, Fire2ProjectileClass );
		GetWorldTimerManager().SetTimer(FiringTimerHandle, ReloadTimerDelegate, ReloadTime, true);
	}
}

void ATankTurret::StopFiring()
{
	bIsFiring = false;
}

