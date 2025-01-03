// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceColonizator.h"

#include "Components/ArrowComponent.h"

#include "Attractor.h"
#include "Node.h"

// Sets default values
ASpaceColonizator::ASpaceColonizator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// transform may not be needed
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");


#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
		ArrowComponent->SetRelativeScale3D(FVector(4.f, 4.f, 4.f));
	}
#endif // WITH_EDITORONLY_DATA


}

void ASpaceColonizator::GrowRootBranch()
{
	const FVector loc = GetActorLocation();
	const FRotator rot = GetActorRotation();

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* r = GetWorld()->SpawnActor(BranchType, &loc, &rot, spawnParams);
	check(r);
	
	// save root branch
	RootBranch = Cast<ANode>(r);
	
	RootBranch->SetSegmentLength(SegmentLength);

	// add root branch to branch array
	Branches.Add(RootBranch);

#if 0
	// debug
	FVector fw = RootBranch->GetActorForwardVector();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Root branch forward : %f, %f, %f"), fw.X, fw.Y, fw.Z));
#endif
}

bool ASpaceColonizator::IsBranchInAnyLeafAttractionDistance(const ANode* branch)
{
	for (const auto& leaf : Leaves)
	{
		float d = leaf->GetDistanceTo(branch);
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
		ANode* child = Branches[i]->GrowChildNode();
		Branches.Add(child);
	}
}

void ASpaceColonizator::GrowTrunk()
{
	ANode* LastBranch = RootBranch;

	while (!IsBranchInAnyLeafAttractionDistance(LastBranch))
	{
		ANode* newBranch = LastBranch->GrowChildNode();
		newBranch->SetSegmentLength(SegmentLength);
		
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
			toRemove->Destroy();
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

	// create root
	GrowRootBranch();

	// generate trunk
	GrowTrunk();

	
	// generate next nodes
}