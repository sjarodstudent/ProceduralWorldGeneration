// Fill out your copyright notice in the Description page of Project Settings.

#include "Node.h"

#include "Attractor.h"

USpaceColonizationNode* USpaceColonizationNode::GrowChildNode(const FVector& defaultGrowDirection)
{
	// spawn a new node in direction of nearby attractors

	FVector dir = FVector::Zero();
	for (int i = 0; i < CurrentNearbyAttractors.Num(); ++i)
	{
		// add direction to the attractor
		dir += CurrentNearbyAttractors[i]->GetActorLocation() - transform.GetLocation();
	}

	// if no nearby attractors, grow based on the parent direction
	if (dir == FVector::ZeroVector)
		dir = defaultGrowDirection;

	dir.Normalize();
	FRotator rot = dir.Rotation().GetNormalized();

	dir *= SegmentLength;

	// spawn the next node based on the computed direction
	USpaceColonizationNode* child = NewObject<USpaceColonizationNode>();
	children.Add(child);
	child->transform.SetLocation(transform.GetLocation() + dir);
	child->transform.SetRotation(rot.Quaternion().GetNormalized());
	child->SegmentLength = SegmentLength;
	child->MaxThickness = MaxThickness;
	child->parent = this;

	CurrentNearbyAttractors.Empty();

	ThickenParent();
	
	return child;
}

void USpaceColonizationNode::ThickenParent()
{
	if (!parent)
		return;
	
	FVector scale = parent->transform.GetScale3D();

	if (scale.Length() >= MaxThickness)
		return;

	parent->transform.SetScale3D(scale * 1.002f);
	parent->ThickenParent();
}
