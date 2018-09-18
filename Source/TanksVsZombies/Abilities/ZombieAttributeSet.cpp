// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAttributeSet.h"
#include "TanksVsZombies.h"



UZombieAttributeSet::UZombieAttributeSet()
	: AttackDamage(0.f)
	, SightDistance(0.f)
	, SightAngle(0.f)
	, AttackDistance(0.f)
	, AttackAngle(0.f)
	, AttackCooldown(0.f)
{

}

void UZombieAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	NewValue = NewValue > 0 ? NewValue : 0;

	Super::PreAttributeChange(Attribute, NewValue);
}

void UZombieAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZombieAttributeSet, AttackDamage);
	DOREPLIFETIME(UZombieAttributeSet, SightDistance);
	DOREPLIFETIME(UZombieAttributeSet, SightAngle);
	DOREPLIFETIME(UZombieAttributeSet, AttackDistance);
	DOREPLIFETIME(UZombieAttributeSet, AttackAngle);
	DOREPLIFETIME(UZombieAttributeSet, AttackCooldown);
}

void UZombieAttributeSet::OnRep_AttackDamage()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, AttackDamage);
}

void UZombieAttributeSet::OnRep_SightDistance()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, SightDistance);
}

void UZombieAttributeSet::OnRep_SightAngle()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, SightAngle);
}

void UZombieAttributeSet::OnRep_AttackDistance()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, AttackDistance);
}

void UZombieAttributeSet::OnRep_AttackAngle()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, AttackAngle);
}

void UZombieAttributeSet::OnRep_AttackCooldown()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UZombieAttributeSet, AttackCooldown);
}
