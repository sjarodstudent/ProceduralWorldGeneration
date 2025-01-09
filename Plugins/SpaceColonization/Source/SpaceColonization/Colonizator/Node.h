// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.generated.h"

class AAttractor;

UCLASS(BlueprintType, Blueprintable)
class SPACECOLONIZATION_API USpaceColonizationNode : public UObject
{
	GENERATED_BODY()
	
private:
	FTransform transform;

	USpaceColonizationNode* parent = nullptr;
	TArray<USpaceColonizationNode*> children;
	
	float SegmentLength = 0.f;
	float MaxThickness = 1.25f;

public:
	TArray<AAttractor*> CurrentNearbyAttractors;

	USpaceColonizationNode* GrowChildNode(const FVector& leavesAverageDirection = FVector::ZeroVector);

public:
	inline bool HasAttractors() const { return CurrentNearbyAttractors.Num() > 0; }

	inline float GetSegmentLength() const { return SegmentLength; }
	inline void SetSegmentLength(const float& segmentLength) { SegmentLength = segmentLength; }

	inline float GetMaxThickness() const { return MaxThickness; }
	inline void SetMaxThickness(const float& maxThickness) { MaxThickness = maxThickness; }

	inline FVector GetLocation() const { return transform.GetLocation(); }
	inline FQuat GetRotation() const { return transform.GetRotation(); }
};
