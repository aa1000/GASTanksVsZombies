// Fill out your copyright notice in the Description page of Project Settings.

#include "TVZGameplayTargetActor_Raduis.h"
#include "Abilities/GameplayAbility.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	TVZGameplayAbilityTargetActor_Radius
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------

ATVZGameplayTargetActor_Raduis::ATVZGameplayTargetActor_Raduis()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = true;

	// Defualt to pawn
	CollisionChannel = ECollisionChannel::ECC_Pawn;
	CollisionProfile = "Pawn";
}

void ATVZGameplayTargetActor_Raduis::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
}

void ATVZGameplayTargetActor_Raduis::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle ATVZGameplayTargetActor_Raduis::MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
	if (OwningAbility)
	{
		/** Use the source location instead of the literal origin */
		return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}

TArray<TWeakObjectPtr<AActor> >	ATVZGameplayTargetActor_Raduis::PerformOverlap(const FVector& Origin)
{
	bool bTraceComplex = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), bTraceComplex);
	Params.bReturnPhysicalMaterial = false;
	Params.bTraceAsyncScene = false;
	TArray<FOverlapResult> Overlaps;

	if(bTraceByProfile)
		SourceActor->GetWorld()->OverlapMultiByProfile(Overlaps, Origin, FQuat::Identity, CollisionProfile,
			FCollisionShape::MakeSphere(Radius), Params);
	else
		SourceActor->GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity,
			FCollisionObjectQueryParams(CollisionChannel), FCollisionShape::MakeSphere(Radius), Params);

	TArray<TWeakObjectPtr<AActor>>	HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		//Should this check to see if these pawns are in the AimTarget list?
		APawn* PawnActor = Cast<APawn>(Overlaps[i].GetActor());
		if (PawnActor && !HitActors.Contains(PawnActor) && Filter.FilterPassesForActor(PawnActor))
		{
			HitActors.Add(PawnActor);
		}
	}

	return HitActors;
}
