// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TVZStatics.generated.h"

/**
 * 
 */
UCLASS()
class TANKSVSZOMBIES_API UTVZStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Calculate the angle diffrence in the range [-180, 182] */
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float FindDeltaAngleDegrees(float A1, float A2);

	/** Finds the angle between two 2D points */
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (Keywords = "rotation rotate"))
	static bool FindLookAtAngle2D(const FVector2D& StartPoint, const FVector2D& TargetPoint, float& Angle);
	
};
