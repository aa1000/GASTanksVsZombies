// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ZombieAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class TANKSVSZOMBIES_API UZombieAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	// Constructor and overrides
	UZombieAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** The damage the zombie does per hit */
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackDamage)
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, AttackDamage)

	/** Sigh distance (when no target in present) */
	UPROPERTY(BlueprintReadOnly, Category = "Sight", ReplicatedUsing = OnRep_SightDistance)
	FGameplayAttributeData SightDistance;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, SightDistance)

	/**  sight half-angle in degrees (when no target is present) */
	UPROPERTY(BlueprintReadOnly, Category = "Sight", ReplicatedUsing = OnRep_SightAngle)
	FGameplayAttributeData SightAngle;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, SightAngle)

	/** The distance from which the zombie starts attacking */
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackDistance)
	FGameplayAttributeData AttackDistance;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, AttackDistance)

	/** Half angle in degrees for out attack */
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackAngle)
	FGameplayAttributeData AttackAngle;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, AttackAngle)

	/** Maximum time between attacks */
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackCooldown)
	FGameplayAttributeData AttackCooldown;
	ATTRIBUTE_ACCESSORS(UZombieAttributeSet, AttackCooldown)

protected:

	UFUNCTION()
	virtual void OnRep_AttackDamage();
	
	UFUNCTION()
	virtual void OnRep_SightDistance();

	UFUNCTION()
	virtual void OnRep_SightAngle();

	UFUNCTION()
	virtual void OnRep_AttackDistance();

	UFUNCTION()
	virtual void OnRep_AttackAngle();

	UFUNCTION()
	virtual void OnRep_AttackCooldown();
};
