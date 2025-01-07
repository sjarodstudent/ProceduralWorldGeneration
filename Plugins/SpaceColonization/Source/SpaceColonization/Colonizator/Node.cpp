// Fill out your copyright notice in the Description page of Project Settings.

#include "Node.h"

#include "Attractor.h"

SpaceColonizationNode::~SpaceColonizationNode()
{
	for (SpaceColonizationNode* child : children)
	{
		delete child;
	}
}

SpaceColonizationNode* SpaceColonizationNode::GrowChildNode(const FVector& leavesAverageDirection)
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
	{
		if (leavesAverageDirection == FVector::ZeroVector)
			// forward vector rotated depending on this node rotation
			dir = FVector::ForwardVector.RotateAngleAxis(transform.GetRotation().GetAngle(),
			                                             transform.GetRotation().GetRotationAxis());
		else
			dir = leavesAverageDirection;
	}

	dir.Normalize();
	FRotator rot = dir.Rotation();

	dir *= SegmentLength;

	// spawn the next node based on the computed direction
	SpaceColonizationNode* child = new SpaceColonizationNode();
	children.Add(child);
	child->transform.SetLocation(transform.GetLocation() + dir);
	child->transform.SetRotation(rot.Quaternion());
	child->SegmentLength = SegmentLength;
	child->MaxThickness = MaxThickness;
	child->parent = this;

	CurrentNearbyAttractors.Empty();

	return child;
}
