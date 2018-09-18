// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATankPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	if(P)
	{
		if (IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(P))
		{
			if(UAbilitySystemComponent* AbilitySystemComp = AbilitySystem->GetAbilitySystemComponent())
				AbilitySystemComp->RefreshAbilityActorInfo();
		}

	}
}
