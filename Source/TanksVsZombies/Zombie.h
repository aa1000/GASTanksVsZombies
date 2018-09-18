// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnWithAbilities.h"
#include "Camera/CameraShake.h"
#include "Zombie.generated.h"

class UZombieAttributeSet;

/**
 * 
 */
UCLASS()
class TANKSVSZOMBIES_API AZombie : public APawnWithAbilities
{
	GENERATED_BODY()

private:

	// The actor we are targeting, NULL if there is not target
	UPROPERTY(VisibleInstanceOnly, Category = AI)
	AActor* TargetActor;

	// The Actor we are targeting, cast to tank and chaced. will be null if not target or can't cast to tank
	UPROPERTY(VisibleInstanceOnly, Category = AI)
	class ATank* TargetTank;

	// current rotation input	
	float YawInput;

	// Current attack button status
	uint32 bAttackInput : 1;

protected:

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	UZombieAttributeSet* ZombieAttributeSet;

	UPROPERTY(EditAnywhere, Category = "Zombie")
	TSubclassOf<UCameraShake> HitCameraShake;

	//// The damage the zombie does per hit
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float AttackDamage;

	//// Sigh distance (when no target in present)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float SightDistance;

	//// sight half-angle in degrees (when no target is present)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float SightAngle;
	//float SightAngleCosine;

	//// Max yaw speed per second in degrees
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//	float YawSpeed;

	//// Speed when wandering around or no target in sight
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float WalkSpeed;

	//// Speed when rushing towards target
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float RunSpeed;

	//// Attack distance
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float AttackDistance;

	//// Half angle in degrees for out attack
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float AttackAngle;
	////float AttackAngleCosine;

	//// Maximum time between attacks
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	//float AttackCooldown;

	float AttackAvailableTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie|Abilities")
	TSubclassOf<class UTVZGameplayAbility> ZombieAttackAbility;

	virtual void ZombieAI(float DeltaSeconds);

	virtual bool ShouldZombieAIAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void ZombieWalk(float DeltaSeconds, const FVector & DistanceWalkedThisFrame);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void ZombieAttack(float DeltaSeconds);

public:
	AZombie();

	virtual void Tick(float DeltaSeconds) override;

	virtual bool CanAttack() const override;

	// Set the Zombie's target. This function will handle setting the TargetActor and TargetTank.
	UFUNCTION(BlueprintCallable, Category = AI)
	void SetTarget(AActor* NewTarget);

	// Returns the TargetActor, NULL means no target
	UFUNCTION(BlueprintCallable, Category = AI)
	FORCEINLINE AActor* GetTarget() { return TargetActor; }

	// Returns the TargetActor as a Tank, NULL means no target or the target is not a tank
	UFUNCTION(BlueprintCallable, Category = AI)
	FORCEINLINE class ATank* GetTargetAsTank() { return TargetTank; }
	

	// Add rotation equal to the Yaw (in degrees) provided
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddRotationInput(float DeltaYawDegrees);

	// Get Rotation input. returns pre-clear value.
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float GetRotationInput() const;

	// Get (and clear) rotation input. Returns pre-clear value.
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float ConsumeRotationInput();

	// Press the attack button, pawn will know what to do with this
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddAttackInput();

	//	Get the statues of the attack button
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool GetAttackInput() const;

	// Get (and clear) the status of the attack button
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool ConsumeAttackInput();

	UFUNCTION(BlueprintPure, Category = "Attributes|Sight")
	float GetSightAngle() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Sight")
	float GetSightDistance() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Attack")
	float GetAttackDamage() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Attack")
	float GetAttackDistance() const;
		
	UFUNCTION(BlueprintPure, Category = "Attributes|Attack")
	float GetAttackAngle() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Attack")
	float GetAttackCooldown() const;

};
