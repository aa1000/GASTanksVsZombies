// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TVZGameplayTargetActor_Raduis.generated.h"

/**
 * Selects everything within a given radius of the source actor using a collision profile.
 */
UCLASS(Blueprintable, notplaceable)
class TANKSVSZOMBIES_API ATVZGameplayTargetActor_Raduis : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:

	ATVZGameplayTargetActor_Raduis();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	/** Radius of target acquisition around the ability's start location. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Radius)
	float Radius;

	/** Trace using this channel if we are not tracing by ObjectType not by profile. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Sweep Type", meta = (EditCondition = "!bTraceByProfile"))
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	/** True for tracing by a collision profile, false for tracing by object type. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Sweep Type")
	uint32 bTraceByProfile : 1;

	/** Trace using this collision profile name if we are tracing by profile. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Sweep Type", meta = (EditCondition="bTraceByProfile"))
	FName CollisionProfile;

protected:

	TArray<TWeakObjectPtr<AActor> >	PerformOverlap(const FVector& Origin);

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;
	
	
};
