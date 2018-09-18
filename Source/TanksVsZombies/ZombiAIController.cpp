// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiAIController.h"
#include "Zombie.h"

void AZombiAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ZombiePawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Zombie Pawn Valid"));
		if(AActor* Target = ZombiePawn->GetTarget())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Zombie Target Valid"));
			// we have a target, move towards it and attack
			FVector DirectionToTarget = (Target->GetActorLocation() - ZombiePawn->GetActorLocation()).GetSafeNormal2D();
			float DotToTarget = FVector::DotProduct(DirectionToTarget, ZombiePawn->GetActorForwardVector());
			float SideWaysDotToTarget = FVector::DotProduct(DirectionToTarget, ZombiePawn->GetActorRightVector());
			float DeltaYawDesired = FMath::Atan2(SideWaysDotToTarget, DotToTarget);

			if(ZombiePawn->CanAttack()) //should be CanZombieAIAttack
				ZombiePawn->AddAttackInput();
			else
			{
				// we scale the value of the input depending on the angle so the further away the target is from in front of the zombie
				// the slower the zombie walks
				const float ScaleValue = FMath::GetMappedRangeValueClamped(FVector2D(-0.707f, 0.707f), FVector2D(0.f, 1.f), DotToTarget);
				// Move Faster when we are moving towards the target so we can turn more accurately
				ZombiePawn->AddMovementInput(FVector(1.f, 1.f, 1.f), ScaleValue);

				// Attempt the entire turn in one frame, the zombie itself will cap this
				ZombiePawn->AddRotationInput(DeltaYawDesired);
			}
		

		
		}

	}
}

void AZombiAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ZombiePawn = Cast<AZombie>(InPawn);
}

void AZombiAIController::UnPossess()
{
	Super::UnPossess();
	ZombiePawn = nullptr;
}
