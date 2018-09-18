// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamgeInterface.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_Unknown				UMETA(DisplayName="Unknown"),
	EDT_HitWithProjectile	UMETA(DisplayName="Hit With Projectile"),
	EDT_Crushed				UMETA(DisplayName="Crushed"),
	EDT_ZombieSlap			UMETA(DisplayName="Zombie Slap")
};
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamgeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKSVSZOMBIES_API IDamgeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveDamage(float IncomingDamage, EDamageType DamageType) = 0;
	virtual float GetHealthRemaining() const = 0;
	
};
