// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/ZombieAttributeSet.h"


AZombie::AZombie()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Attributes setup
	//SightDistance = 500.f;
	//SightAngle = 60.f;
	//YawSpeed = 90.f;
	//WalkSpeed = 25.f;
	//RunSpeed = 45.f;
	//AttackDistance = 100.f;
	//AttackAngle = 30.f;
	//AttackCooldown = 1.f;
	//AttackDamage = 10.f;

	// Create the attribute set, this replicates by default
	ZombieAttributeSet = CreateDefaultSubobject<UZombieAttributeSet>(TEXT("ZombieAttributeSet"));

}

void AZombie::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetHealth() > 0)
	{
		ZombieAI(DeltaSeconds);

		// Clear out leftover movement 
		ConsumeMovementInputVector();
		ConsumeRotationInput();
		ConsumeAttackInput();
	}
}

void AZombie::ZombieAI(float DeltaSeconds)
{
	// the zombie always moves unless it's attacking. If it moving, it moves between WalkSpeed and RunSpeed

//	const FVector WalkingMovement = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(WalkSpeed, RunSpeed),
//		GetPendingMovementInputVector().X) * DeltaSeconds * GetActorForwardVector();

	const FVector WalkingMovement = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(GetMoveSpeed(), GetMoveSpeed()*2),
		GetPendingMovementInputVector().X) * DeltaSeconds * GetActorForwardVector();

	FVector DesiredMovement = GetAttackInput() ? FVector::ZeroVector : WalkingMovement;
	FVector OriginalLocation = GetActorLocation();
	FVector DesiredLocation = OriginalLocation + DesiredMovement;
	//float MaxYawThisFrame = YawSpeed * DeltaSeconds;
	const float MaxYawThisFrame = GetYawSpeed() * DeltaSeconds;
	FRotator DesiredRotation = GetActorRotation() +
		FRotator(0.f, FMath::Clamp<float>(GetRotationInput(), -MaxYawThisFrame, MaxYawThisFrame), 0.f);

	SetActorLocationAndRotation(DesiredLocation, DesiredRotation.Quaternion(), true);
	FVector DistanceWalked = GetActorLocation() - OriginalLocation;

	if(!DistanceWalked.IsNearlyZero())
		ZombieWalk(DeltaSeconds, DistanceWalked);

	
	// see if we have a target and deal with it if we do. Find a Target if we don't
	if(TargetActor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Zombie has a target"));
		// we have a target, see if we a re attacking since our attacks always hit
		FVector OurLocation = GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		FVector DirectionToTarget = (TargetLocation - OurLocation).GetSafeNormal2D();
		float DotToTarget = FVector::DotProduct(DirectionToTarget, GetActorForwardVector());
		
		//float CurrentTime = GetWorld()->GetTimeSeconds();
		// Check to see if we should attack based on button status and attack cooldown
		if(GetAttackInput())// && AttackAvailableTime <= CurrentTime)
		{
			//AttackAvailableTime = CurrentTime + AttackCooldown;
			ZombieAttack(DeltaSeconds);

			//UE_LOG(LogTemp, Warning, TEXT("Dot to target: %f"), DotToTarget);
			//UE_LOG(LogTemp, Warning, TEXT("Attack angle: %f"), FMath::Cos(FMath::DegreesToRadians(AttackAngle)));
			if( DotToTarget >= FMath::Cos( FMath::DegreesToRadians( GetAttackAngle() ) ) )
			{
				//UE_LOG(LogTemp, Warning, TEXT("Our Location: X=%f, Y=%f, Z=%f"), OurLocation.X, OurLocation.Y, OriginalLocation.Z);
				//UE_LOG(LogTemp, Warning, TEXT("Target Location: X=%f, Y=%f, Z=%f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z);

				//UE_LOG(LogTemp, Warning, TEXT("Zombie within attack angle"));
				float DistSqXY = FVector::DistSquaredXY(TargetActor->GetActorLocation(), OurLocation);
				const float AttackDistance = GetAttackDistance();
				if(DistSqXY <= AttackDistance * AttackDistance)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Zombie in distance to attack"));
					if(TargetTank)
					{
						//UE_LOG(LogTemp, Warning, TEXT("Zombie Tank target Vaild"));
						//TargetTank->ReceiveDamage(AttackDamage, EDamageType::EDT_ZombieSlap);
						
						if(AbilitySystemComponent)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Zombie ASC valid"));
							if(AbilitySystemComponent->TryActivateAbilityByClass(ZombieAttackAbility))
							{
								if (APlayerController* TankPC = Cast<APlayerController>(TargetTank->GetController()))
									TankPC->ClientPlayCameraShake(HitCameraShake, 1.0f);
							}
						}

						
					}
					else
						SetTarget(nullptr);
				}
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Zombie looking for a target"));
		// Look for a target.
		AActor* Target = UGameplayStatics::GetPlayerPawn(this, 0);

		if(Target)
		{
			float DistSqXY = FVector::DistSquaredXY(Target->GetActorLocation(), GetActorLocation());
			const float SightDistance = GetSightDistance();
			if (DistSqXY <= (SightDistance * SightDistance))
			{
				FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				if (FVector::DotProduct(DirectionToTarget, GetActorForwardVector()) >= FMath::Cos(FMath::DegreesToRadians(GetSightAngle())))
				{
					SetTarget(Target);
				}
			}
		}
	}
}

bool AZombie::ShouldZombieAIAttack()
{
	return false;
}

bool AZombie::CanAttack() const
{
	if(TargetActor)
	{
		// Attack our Target if we are in range, distance and angle
		FVector OurLocation = GetActorLocation();
		FVector DirectionToTarget = (TargetActor->GetActorLocation() - OurLocation).GetSafeNormal2D();
		float DotToTarget = FVector::DotProduct(DirectionToTarget, GetActorForwardVector());
		const float AttackAngle = GetAttackAngle();
		const float AttackDistance = GetAttackDistance();
		if( DotToTarget >= FMath::Cos(FMath::DegreesToRadians(AttackAngle)) )
		{
			float DistSqXY = FVector::DistSquaredXY(TargetActor->GetActorLocation(), OurLocation);

			if(DistSqXY <= (AttackDistance * AttackDistance))
				// attack cooldown is not checked here
				return true;
		}
	}
	return false;
}

void AZombie::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;

	TargetTank = Cast<ATank>(NewTarget);
}

void AZombie::AddRotationInput(float DeltaYawDegrees)
{
	YawInput += DeltaYawDegrees;
}

float AZombie::GetRotationInput() const
{
	return YawInput;
}

float AZombie::ConsumeRotationInput()
{
	const float RetVal = YawInput;
	YawInput = 0.f;
	return RetVal;
}

void AZombie::AddAttackInput()
{
	bAttackInput = true;
}

bool AZombie::GetAttackInput() const
{
	return bAttackInput;
}

bool AZombie::ConsumeAttackInput()
{
	const bool bRetVal = bAttackInput;
	bAttackInput = false;
	return bRetVal;
}

float AZombie::GetSightAngle() const
{
	return ZombieAttributeSet->GetSightAngle();
}

float AZombie::GetSightDistance() const
{
	return ZombieAttributeSet->GetSightDistance();
}

float AZombie::GetAttackDamage() const
{
	return ZombieAttributeSet->GetAttackDamage();
}

float AZombie::GetAttackDistance() const
{
	return ZombieAttributeSet->GetAttackDistance();
}

float AZombie::GetAttackAngle() const
{
	return ZombieAttributeSet->GetAttackAngle();
}

float AZombie::GetAttackCooldown() const
{
	return ZombieAttributeSet->GetAttackCooldown();
}

