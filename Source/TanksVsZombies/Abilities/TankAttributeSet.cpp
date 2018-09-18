// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAttributeSet.h"
#include "TanksVsZombies.h"
#include "GameplayEffectExtension.h"


UTankAttributeSet::UTankAttributeSet()
	: TurretYawSpeed(0.f)
{

}

void UTankAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	NewValue = NewValue > 0 ? NewValue : 0;

	Super::PreAttributeChange(Attribute, NewValue);

}

void UTankAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

void UTankAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTankAttributeSet, TurretYawSpeed);
}

void UTankAttributeSet::OnRep_TurretYawSpeed()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTankAttributeSet, TurretYawSpeed);
}
