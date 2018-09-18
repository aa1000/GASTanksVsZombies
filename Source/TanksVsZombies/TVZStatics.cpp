// Fill out your copyright notice in the Description page of Project Settings.

#include "TVZStatics.h"


float UTVZStatics::FindDeltaAngleDegrees(float A1, float A2)
{
	return FMath::FindDeltaAngleDegrees(A1, A2);
}

bool UTVZStatics::FindLookAtAngle2D(const FVector2D& StartPoint, const FVector2D& TargetPoint, float& Angle)
{
	FVector2D Normal = (TargetPoint - StartPoint);//.GetSafeNormal();

	if(Normal.IsNearlyZero())
		return false;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(Normal.Y, Normal.X));
	return true;
}
