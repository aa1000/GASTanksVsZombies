// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "WorldCollision.h"
#include "DamgeInterface.h"
//#include "TimerManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ProjectileBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Body"));
	ProjectileBody->SetupAttachment(RootComponent);
	ProjectileBody->SetCollisionProfileName("NoCollision");
	//ProjectileBody->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	ProjectileSpeed = 200.f;
	ProjectileDamage = 50.f;
	TimeToExplode = 1.f;
	ExplosionRaduis = 50.f;
	MovementCollisionProfile = TEXT("ProjectileFly");


	//GameplayEventHitTag = TEXT("Ability.Projectile.Hit");
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &AProjectile::Explode, TimeToExplode);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned"));
}


//void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Projectile Hit"));
//	SetActorEnableCollision(false);
//	SetActorLocation(Hit.Location);
//	Explode();
//}

void AProjectile::Explode()
{

	//TArray<FHitResult> OutHits;
	//const FVector StartLocation = GetActorLocation();
	//const FVector EndLocation(StartLocation.X, StartLocation.Y, StartLocation.Z + 1);
	//FCollisionShape CollisionShape;
	//CollisionShape.SetCapsule(ExplosionRaduis*2, 200.f);
	//FCollisionQueryParams QCP;
	//QCP.AddIgnoredActor(this);
	//QCP.bReturnPhysicalMaterial = false;
	//GetWorld()->SweepMultiByProfile(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
	//	MovementCollisionProfile, CollisionShape, QCP);

	//for(const FHitResult& Hit : OutHits)
	//	if (IDamgeInterface* DmgInterface = Cast<IDamgeInterface>(Hit.GetActor()))
	//		DmgInterface->ReceiveDamage(ProjectileDamage/2, EDamageType::EDT_HitWithProjectile);

	FGameplayAbilityTargetingLocationInfo TargetDataLocationInfo;
	TargetDataLocationInfo.LiteralTransform.SetLocation(GetActorLocation());

	FGameplayEventData Data;
	//Data.Target = HitActor;
	Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(TargetDataLocationInfo, TargetDataLocationInfo);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, GameplayEventHitTag, Data);

	OnExplode();
}

void AProjectile::OnExplode_Implementation()
{
	Destroy();
}


void AProjectile::HandleHitActor(AActor* HitActor)
{
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Hit Actor"));

	//if(IDamgeInterface* DmgInterface = Cast<IDamgeInterface>(HitActor))
	//	DmgInterface->ReceiveDamage(ProjectileDamage, EDamageType::EDT_HitWithProjectile);

	FGameplayAbilityTargetingLocationInfo TargetDataLocationInfo;
	TargetDataLocationInfo.LiteralTransform.SetLocation(GetActorLocation());

	FGameplayEventData Data;
	Data.Target = HitActor;
	Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(TargetDataLocationInfo, TargetDataLocationInfo);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, GameplayEventHitTag, Data);

	OnExplode();
}

void AProjectile::MoveProjectile(float DeltaTime)
{
	FVector Loc = GetActorLocation();
	FVector DesiredLocation = Loc + (DeltaTime * ProjectileSpeed) * GetTransform().GetUnitAxis(EAxis::X);
	SetActorLocation(DesiredLocation);


	if(UWorld* World = GetWorld())
	{
		FHitResult OutHit;
		FCollisionShape CollisionShape;
		//CollisionShape.SetCapsule(20, 200.f);
		if(GetActorEnableCollision() && World->SweepSingleByProfile(OutHit, Loc,
			DesiredLocation, FQuat::Identity, MovementCollisionProfile, ProjectileBody->GetCollisionShape()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Projectile Hit Sweep"));
			// If we collided with something 
			SetActorEnableCollision(false);
			SetActorLocation(OutHit.Location);
			HandleHitActor(OutHit.GetActor());
			Explode();
		}
		else 
			// If we didn't collide, just go to the location we want
			SetActorLocation(DesiredLocation);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveProjectile(DeltaTime);
}

