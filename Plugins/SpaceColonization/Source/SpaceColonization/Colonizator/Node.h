// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

class AAttractor;

class SPACECOLONIZATION_API SpaceColonizationNode
{
private:
	FTransform transform;

	SpaceColonizationNode* parent = nullptr;
	TArray<SpaceColonizationNode*> children;
	
	float SegmentLength = 0.f;
	float MaxThickness = 1.25f;

public:
	~SpaceColonizationNode();
	
	TArray<AAttractor*> CurrentNearbyAttractors;

	SpaceColonizationNode* GrowChildNode(const FVector& leavesAverageDirection = FVector::ZeroVector);

public:
	inline bool HasAttractors() const { return CurrentNearbyAttractors.Num() > 0; }

	inline float GetSegmentLength() const { return SegmentLength; }
	inline void SetSegmentLength(const float& segmentLength) { SegmentLength = segmentLength; }

	inline float GetMaxThickness() const { return MaxThickness; }
	inline void SetMaxThickness(const float& maxThickness) { MaxThickness = maxThickness; }
};
