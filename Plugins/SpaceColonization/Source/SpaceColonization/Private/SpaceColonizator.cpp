// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceColonizator.h"

#include "Attractor.h"
#include "Node.h"

// Sets default values
ASpaceColonizator::ASpaceColonizator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// transform may not be needed
	// RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

bool ASpaceColonizator::IsAnyBranchInAttractionDistance()
{
	for (const auto& leaf : Leaves)
	{
		float d = leaf->GetDistanceTo(RootBranch);
		if (d <= leaf->AttractionDistance)
			return true;
	}
	return false;
}


void ASpaceColonizator::GrowBranches()
{
	// generate next nodes
	
	int constSize = Branches.Num();
	for (int i = 0; i < constSize; ++i)
	{
		ANode* child = Branches[i]->GenerateChildNode();
		Branches.Add(child);
	}
}

void ASpaceColonizator::GenerateTrunk()
{
	ANode* LastBranch = RootBranch;

	while (!IsAnyBranchInAttractionDistance())
	{
		ANode* newBranch = LastBranch->GenerateChildNode();
		Branches.Add(newBranch);
		LastBranch = newBranch;
	}
	
}

void ASpaceColonizator::ProcessLeaves()
{
	// link attractors to their closest node

	for (int i = 0; i < Leaves.Num(); ++i)
	{
		float minDistance = TNumericLimits<float>::Max();
		for (int j = 0; j < Branches.Num(); ++j)
		{
			float d = FVector::Dist(Branches[j]->GetActorLocation(), Leaves[i]->GetActorLocation());
			if (d < minDistance)
			{
				minDistance = d;

				// Closest branch
				Leaves[i]->CurrentAttractedNode = Branches[j];
			}
		}

		if (Leaves[i]->IsReached())
		{
			AAttractor* toRemove = Leaves[i];
			Leaves.Remove(toRemove);
			Destroy(toRemove);
		}
		
		if (Leaves[i]->IsInAttractionRange())
			Leaves[i]->CurrentAttractedNode->CurrentNearbyAttractors.Add(Leaves[i]);
		else
			Leaves[i]->CurrentAttractedNode = nullptr;
	}
}

// Called when the game starts or when spawned
void ASpaceColonizator::BeginPlay()
{
	Super::BeginPlay();

	if (!Branches.IsEmpty())
	{
		RootBranch = Branches[0];
		RootBranch->SegmentLength = SegmentLength;
	}

	
	// generate next node

	GrowBranches();
}