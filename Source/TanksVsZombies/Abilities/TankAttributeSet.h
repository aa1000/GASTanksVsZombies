// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
//#include "Abilities/TVZAttributeSetBase.h"
#include "TankAttributeSet.generated.h"


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
class TANKSVSZOMBIES_API UTankAttributeSet : public UAttributeSet//UTVZAttributeSetBase
{
	GENERATED_BODY()
	
	
public:

	// Constructor and overrides
	UTankAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** The speed at which the turret can turn (degrees/sec) */
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_TurretYawSpeed)
	FGameplayAttributeData TurretYawSpeed;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, TurretYawSpeed)



protected:

	UFUNCTION()
	virtual void OnRep_TurretYawSpeed();
};
