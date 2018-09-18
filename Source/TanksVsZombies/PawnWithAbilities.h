// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Abilities/TVZGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DamgeInterface.h"
#include "PawnWithAbilities.generated.h"

class UTVZGameplayAbility;
class UTVZAttributeSetBase;

UCLASS()
class TANKSVSZOMBIES_API APawnWithAbilities : public APawn, public IAbilitySystemInterface//, public IDamgeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnWithAbilities();

protected:


	// Current pawn health
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (ClampMin = "0.0"))
	//float Health;

	// The abilities to give to this pawn when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UTVZGameplayAbility>> AbilitiesToGive;

	// The effects to give to this pawn when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

	//// The attribute set to give this pawn
	//UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	//TArray<TSubclassOf<UTVZAttributeSetBase>> PawnAttributeSet;

	// Shows the pawn direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Direction)
	class UArrowComponent* Direction;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly, Category = Abilities)
	UAbilitySystemComponent* AbilitySystemComponent;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	UTVZAttributeSetBase* BaseAttributeSet;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	//class UDataTable* AttrDataTable;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void AddStartupAbilitiesAndEffects();

	// The base version of this function calls Death()
	UFUNCTION(BlueprintNativeEvent, Category = "Pawn With Abilities")
	void OnDeath();

	// Handles the death of the pawn, the base version of this function just destroys the actor
	virtual void HandleDeath();

	// handle taking damage in blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Pawn With Abilities")
	void OnDamaged(float DamageDone, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
		APawnWithAbilities* SourceCharacter, AActor* SourceActor);

	// handle health change in blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Pawn With Abilities")
	void OnHealthChanged(float HealthChangeAmount, const FGameplayTagContainer& SourceTags);

public:

	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	float GetHealingMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Movement")
	float GetYawSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Movement")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Damage")
	float GetDefensePower() const;

	UFUNCTION(BlueprintPure, Category = "Attributes|Damage")
	float GetDamageMultiplier() const;

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanAttack() const;
	
	//~ the ~ makes this not appear as a tooltip
	//~ Begin IDamgeInterface
	//virtual void ReceiveDamage(float IncomingDamage, EDamageType DamageType) override;
	//virtual float GetHealthRemaining() const override;
	//~ End IDamgeInterface

	virtual void HandleDamage(float DamageDone, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags,
		APawnWithAbilities* SourceCharacter, AActor* SourceActor);

	virtual void HandleHealthChanged(float HealthChangeAmount, const FGameplayTagContainer& SourceTags);

};
