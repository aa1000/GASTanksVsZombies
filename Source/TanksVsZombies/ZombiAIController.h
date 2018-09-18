// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombiAIController.generated.h"

class AZombie;
/**
 * 
 */
UCLASS()
class TANKSVSZOMBIES_API AZombiAIController : public AAIController
{
	GENERATED_BODY()


protected:

	// Our pawn, precast to a Zombie, will be NULL is the pawn is null or not a Zombie
	UPROPERTY(BlueprintREadOnly, Category = Zombie)
	AZombie* ZombiePawn;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	virtual void UnPossess() override;
	
};
