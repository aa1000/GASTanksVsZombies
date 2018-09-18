// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnWithAbilities.h"
#include "Components/ArrowComponent.h"
#include "Abilities//TVZGameplayAbility.h"
#include "Abilities/TVZAttributeSetBase.h"
#include "AbilitySystemGlobals.h"

// Sets default values
APawnWithAbilities::APawnWithAbilities()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	Direction->SetupAttachment(RootComponent);

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	BaseAttributeSet = CreateDefaultSubobject<UTVZAttributeSetBase>(TEXT("AttributeSet"));

	//Health = 100.f;
}

// Called when the game starts or when spawned
void APawnWithAbilities::BeginPlay()
{
	Super::BeginPlay();
	
}


void APawnWithAbilities::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// init ability system
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// assign attribute set
		//if(AttrDataTable)
		//	AbilitySystemComponent->InitStats(UTVZAttributeSetBase::StaticClass(), AttrDataTable);

		// give abilities
		AddStartupAbilitiesAndEffects();
	}
}

void APawnWithAbilities::AddStartupAbilitiesAndEffects()
{
	if (HasAuthority())
	{
		for (TSubclassOf<UTVZGameplayAbility>&  AbilityToGive : AbilitiesToGive)
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGive));

		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (NewHandle.IsValid())
			{
				//FActiveGameplayEffectHandle ActiveGEHandle =
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
}

float APawnWithAbilities::GetHealth() const
{
	return BaseAttributeSet->GetHealth();
}

float APawnWithAbilities::GetMaxHealth() const
{
	return BaseAttributeSet->GetMaxHealth();
}

float APawnWithAbilities::GetHealingMultiplier() const
{
	return BaseAttributeSet->GetHealingMultiplier();
}

float APawnWithAbilities::GetYawSpeed() const
{
	return BaseAttributeSet->GetYawSpeed();
}

float APawnWithAbilities::GetMoveSpeed() const
{
	return BaseAttributeSet->GetMoveSpeed();
}

float APawnWithAbilities::GetDefensePower() const
{
	return BaseAttributeSet->GetDefensePower();
}

float APawnWithAbilities::GetDamageMultiplier() const
{
	return BaseAttributeSet->GetDamageMultiplier();
}

class UAbilitySystemComponent* APawnWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called every frame
void APawnWithAbilities::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnWithAbilities::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APawnWithAbilities::CanAttack() const
{
	if(GetHealth() > 0)
		return true;
	return false;
}

void APawnWithAbilities::HandleDamage(float DamageDone, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
	APawnWithAbilities* SourceCharacter, AActor* SourceActor)
{
	OnDamaged(DamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
}

void APawnWithAbilities::HandleHealthChanged(float HealthChangeAmount, const FGameplayTagContainer& SourceTags)
{
	OnHealthChanged(HealthChangeAmount, SourceTags);
	if (GetHealth() <= 0)
		OnDeath();
}

//void APawnWithAbilities::ReceiveDamage(float IncomingDamage, EDamageType DamageType)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("Incoming Damage: %.2f"), IncomingDamage);
//
//	if (IncomingDamage < 0)
//		return;
//
//	Health = FMath::Clamp<float>(Health - IncomingDamage, 0.f, 100.f);
//
//	if (Health <= 0)
//		OnDeath();
//}
//
//float APawnWithAbilities::GetHealthRemaining() const
//{
//	return Health;
//}

void APawnWithAbilities::OnDeath_Implementation()
{
	HandleDeath();
}

void APawnWithAbilities::HandleDeath()
{
	Destroy();
}

