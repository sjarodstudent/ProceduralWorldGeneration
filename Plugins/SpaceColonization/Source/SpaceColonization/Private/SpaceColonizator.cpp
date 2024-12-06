// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceColonizator.h"

#include "Attractor.h"
#include "IntVectorTypes.h"
#include "Node.h"
#include "VectorTypes.h"
#include "Particles/Attractor/ParticleModuleAttractorParticle.h"

// Sets default values
ASpaceColonizator::ASpaceColonizator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

#define DebugLog(seconds, color, ...) GEngine->AddOnScreenDebugMessage(-1, seconds, color, FString::Printf(__VA_ARGS__))

#define DebugLogRed(...) DebugLog(15.f, FColor::Red, __VA_ARGS__)
#define DebugLogGreen(...) DebugLog(15.f, FColor::Green, __VA_ARGS__)
#define DebugLogBlue(...) DebugLog(15.f, FColor::Cyan, __VA_ARGS__)

#define DebugLogPerFrame(color, ...) DebugLog(DeltaTime, color, __VA_ARGS__)

// Called when the game starts or when spawned
void ASpaceColonizator::BeginPlay()
{
	Super::BeginPlay();

	if (!Branches.IsEmpty())
	{
		RootBranch = Branches[0];
		RootBranch->SegmentLength = SegmentLength;
	}

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
				Leaves[i]->CurrentAttractedNode = Branches[j];
			}
		}
		Leaves[i]->CurrentAttractedNode->CurrentNearbyAttractors.Add(Leaves[i]);
		DebugLogRed(TEXT("%f, %f, %f"), Leaves[i]->GetActorLocation().X,
			Leaves[i]->GetActorLocation().Y,
			Leaves[i]->GetActorLocation().Z);
	}

	// generate next node

	int constSize = Branches.Num();
	for (int i = 0; i < constSize; ++i)
	{
		ANode* child = Branches[i]->GenerateChildNode();
		DebugLogBlue(TEXT("%f, %f, %f"), child->GetActorLocation().X,
	child->GetActorLocation().Y,
	child->GetActorLocation().Z);

		Branches.Add(child);
	}
}

// Called every frame
void ASpaceColonizator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
