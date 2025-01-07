// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceColonizator.h"


#include "Components/ArrowComponent.h"

#include "Node.h"
#include "Actor/BranchActor.h"

#include "Attractor.h"
#include "AttractorCloud.h"

// Sets default values
ASpaceColonizator::ASpaceColonizator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = bGrowTemporally;

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

	// ensure the array is empty
	if (!Branches.IsEmpty())
		Branches.Empty();
}

ASpaceColonizator::~ASpaceColonizator()
{
	for (SpaceColonizationNode* branch : Branches)
	{
		delete Branches;
	}
}

void ASpaceColonizator::LinkAttractorCloud()
{
	if (!LeafCloud)
		return;

	if (LeafCloud->GetAttractorArray().IsEmpty())
		LeafCloud->GenerateAttractors();

	Leaves.Append(LeafCloud->GetAttractorArray());
}


void ASpaceColonizator::GrowRootBranch()
{
	const FVector loc = GetActorLocation();
	const FRotator rot = GetActorRotation();

	// save root branch
	RootBranch = new SpaceColonizationNode();
	RootBranch->SetSegmentLength(SegmentLength);
	RootBranch->SetMaxThickness(MaxThickness);

	// ensure the array is empty (it should but it is not)
	if (!Branches.IsEmpty())
		Branches.Empty();

	// add root branch to branch array
	Branches.Add(RootBranch);

#if 0
	// debug
	FVector fw = RootBranch->GetActorForwardVector();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Root branch forward : %f, %f, %f"), fw.X, fw.Y, fw.Z));
#endif

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* r = GetWorld()->SpawnActor(BranchType, &loc, &rot, spawnParams);
	check(r);
}

bool ASpaceColonizator::IsBranchInAnyLeafAttractionDistance(const SpaceColonizationNode* branch) const
{
	for (const auto& leaf : Leaves)
	{
		float d = leaf->GetDistanceTo(branch);
		if (d <= leaf->GetAttractionDistance())
			return true;
	}
	return false;
}


bool ASpaceColonizator::IsAnyBranchInAnyLeafAttractionDistance() const
{
	for (const auto& branch : Branches)
	{
		if (IsBranchInAnyLeafAttractionDistance(branch))
			return true;
	}
	return false;
}

FVector ASpaceColonizator::GetLeavesAverageLocation() const
{
	FVector dir = FVector::ZeroVector;
	for (const auto& leaf : Leaves)
	{
		dir += leaf->GetActorLocation();
	}
	dir.Normalize();
	return dir;
}

void ASpaceColonizator::GrowTrunk()
{
	SpaceColonizationNode* LastBranch = RootBranch;

	FVector leavesAverageLocation = FVector::ZeroVector;
	if (!bTrunkFollowArrow)
		leavesAverageLocation = GetLeavesAverageLocation();

	while (!IsBranchInAnyLeafAttractionDistance(LastBranch))
	{
		SpaceColonizationNode* newBranch = LastBranch->GrowChildNode(leavesAverageLocation);
		newBranch->SetOptions(SegmentLength, MaxThickness);

		Branches.Add(newBranch);
		LastBranch = newBranch;
	}
}

void ASpaceColonizator::ProcessLeaves()
{
	// link attractors to their closest node

	for (int i = 0; i < Leaves.Num(); ++i)
	{
		AAttractor* leaf = Leaves[i];
		leaf->Reset();

		float minDistance = TNumericLimits<float>::Max();

		for (int j = 0; j < Branches.Num(); ++j)
		{
			SpaceColonizationNode* branch = Branches[j];

			float d = FVector::Dist(branch->GetActorLocation(), leaf->GetActorLocation());
			if (d < minDistance)
			{
				minDistance = d;

				// Closest branch
				leaf->CurrentAttractedNode = branch;
			}
		}

		if (leaf->IsReached())
		{
			AAttractor* toRemove = leaf;
			Leaves.Remove(toRemove);
			toRemove->Destroy();
		}

		if (leaf->IsInAttractionRange())
			leaf->CurrentAttractedNode->CurrentNearbyAttractors.Add(leaf);
		else
			leaf->Reset();
	}
}

void ASpaceColonizator::GrowBranches()
{
	// generate next nodes

	int constSize = Branches.Num();
	for (int i = 0; i < constSize; ++i)
	{
		SpaceColonizationNode* branch = Branches[i];

		if (branch->HasAttractors())
		{
#if 0
			for (int j = 0; j < branch->CurrentNearbyAttractors.Num(); ++j)
			{
				AAttractor* attractor = branch->CurrentNearbyAttractors[j];
				FVector start = attractor->GetActorLocation();
				FVector end = branch->GetActorLocation();
				DrawDebugLine(GetWorld(), start, end, FColor::Blue, true, 0.1f, 0, 5.f);
			}
#endif

			SpaceColonizationNode* child = branch->GrowChildNode();
			Branches.Add(child);
		}
	}
}


// Called when the game starts or when spawned
void ASpaceColonizator::BeginPlay()
{
	Super::BeginPlay();

	LinkAttractorCloud();

	// create root
	GrowRootBranch();

	// generate trunk
	GrowTrunk();


	// generate next nodes

	if (bGrowTemporally)
	{
		while (IsAnyBranchInAnyLeafAttractionDistance())
		{
			ProcessLeaves();
			GrowBranches();
		}
		Leaves.Empty();
	}
}

void ASpaceColonizator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGrowTemporally)
	{
		if (GrowTimerStamp >= GrowTimer)
		{
			GrowTimerStamp = 0.f;

			ProcessLeaves();
			GrowBranches();
		}

		GrowTimerStamp += DeltaSeconds;
	}
}
