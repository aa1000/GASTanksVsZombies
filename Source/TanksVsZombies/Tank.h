// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnWithAbilities.h"
#include "Tank.generated.h"

class UTVZGameplayAbility;

/**	this struct covers all possible tank input schemes.
 *	what inputs do can vary by tank but the same inputs will always exist.
 */
USTRUCT(BlueprintType)
struct FTankInput
{
	GENERATED_BODY()

public:
	// sanitizing movemet input (aka not going twice as fast when holding two input keys)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FVector2D MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	uint32 bFire1 : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	uint32 bFire2 : 1;

	void Sanitize()
	{
		MovementInput = RawMovementInput.ClampAxes(-1.f, 1.f);
		MovementInput = MovementInput.GetSafeNormal();
		RawMovementInput = FVector2D::ZeroVector;
	}

	void MoveX(float Value)
	{
		RawMovementInput.X += Value;
	}

	void MoveY(float Value)
	{
		RawMovementInput.Y += Value;
	}

	void Fire1(bool bPressed)
	{
		bFire1 = bPressed;
	}

	void Fire2(bool bPressed)
	{
		bFire2 = bPressed;
	}

private:
	FVector2D RawMovementInput;
};

/**
 * 
 */
UCLASS()
class TANKSVSZOMBIES_API ATank : public APawnWithAbilities
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);
public:
	ATank();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Attributes|Movement")
	float GetTurretYawSpeed() const;

	FORCEINLINE const FTankInput& GetTankInput() const { return TankInput;	}

protected:

	//	Sprite for the Tank body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	class UPaperSpriteComponent* TankSprite;

	// The actor used as the turret
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	UChildActorComponent* ChildTurret;

	// Collision body for the tank
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	class UBoxComponent* TankBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FTankInput TankInput;

	//// Max turn rate (degrees/sec) for the tank
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement", meta = (ClampMin = "0.0"))
	//float YawSpeed;

	//// Max speed (units/sec) for the tank
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement", meta = (ClampMin = "0.0"))
	//float MoveSpeed;

	// Max turret turn rate (degrees/sec) for rotating the turret
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement", meta = (ClampMin = "0.0"))
	//float TurretYawSpeed;

	// Name of the collision profile for crushing zombies
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement")
	//FName CrushCollisionProfile;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	class UTankAttributeSet* TankAttributeSet;

	// The class of ability this turret can fire on fire 1
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UTVZGameplayAbility> Fire1AbilityClass;

	// The class of ability this turret can fire on fire 2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UTVZGameplayAbility> Fire2AbilityClass;

	// The class of ability to crush zombies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UTVZGameplayAbility> CrushZombiesAbilityClass;

	// Name of the tag that the gameplay event waiting for hit has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	FGameplayTag GameplayEventHitZombieTag;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void MoveX(float Value);
	
	virtual void MoveY(float Value);

	virtual void MoveTank(float DeltaTime);

	virtual void Fire1Pressed();
	virtual void Fire1Released();

	virtual void Fire2Pressed();
	virtual void Fire2Released();

	virtual void OnOverlapPawnWithAbilities(APawnWithAbilities* OtherPawn);

	virtual void HandleDeath() override;
};
