// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceColonizator.h"


#include "Components/ArrowComponent.h"

#include "Node.h"
#include "Actor/BranchActor.h"

#include "Attractor.h"
#include "AttractorCloud.h"
#include "MovieSceneTracksComponentTypes.h"

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
	// save root branch
	RootBranch = NewObject<USpaceColonizationNode>();
	RootBranch->SetLocation(GetActorLocation());
	RootBranch->SetRotation(GetActorRotation());
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

	OnBranchSpawnedDo(RootBranch);
}

bool ASpaceColonizator::IsBranchInAnyLeafAttractionDistance(const USpaceColonizationNode* branch) const
{
	for (const auto& leaf : Leaves)
	{
		float d = leaf->GetDistanceToBranch(branch);
		if (d <= leaf->GetAttractionDistance())
			return true;
	}
	return false;
}


bool ASpaceColonizator::IsAnyBranchInAnyLeafAttractionDistance() const
{
	for (const auto& branch : Branches)
	{
		if (!branch->HasChildren() && IsBranchInAnyLeafAttractionDistance(branch))
			return true;
	}
	return false;
}

float ASpaceColonizator::GetFurthestLeafFromCloudDistance() const
{
	FVector center = GetLeavesAverageLocation();
	float maxDistance = TNumericLimits<float>::Min();
	for (const auto& leaf : Leaves)
	{
		float d = FVector::Dist(center, leaf->GetActorLocation());
		maxDistance = FMath::Max(maxDistance, d);
	}

	return maxDistance;
}

AAttractor* ASpaceColonizator::GetClosestLeafFromBranch(const USpaceColonizationNode* branch) const
{
	float minDistance = TNumericLimits<float>::Max();
	AAttractor* closestLeaf = nullptr;
	for (const auto& leaf : Leaves)
	{
		float d = FVector::Dist(branch->GetLocation(), leaf->GetActorLocation());
		if (d < minDistance)
		{
			minDistance = d;
			closestLeaf = leaf;
		}
	}
	return closestLeaf;
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
	USpaceColonizationNode* LastBranch = RootBranch;

	FVector trunkDir = GetActorForwardVector();
	if (!bTrunkFollowArrow)
		trunkDir = GetLeavesAverageLocation();


	// compute cloud radius from furthest leaf from center of cloud
	// because maybe the cloud is hand made, not with the cloud object class
	float cloudRadius = GetFurthestLeafFromCloudDistance();

	while (!IsBranchInAnyLeafAttractionDistance(LastBranch))
	{
		USpaceColonizationNode* newBranch = LastBranch->GrowChildNode(trunkDir);
		newBranch->SetSegmentLength(SegmentLength);
		newBranch->SetMaxThickness(MaxThickness);

		Branches.Add(newBranch);
		LastBranch = newBranch;
		OnBranchSpawnedDo(newBranch);

		float d = FVector::Dist(LastBranch->GetLocation(), GetLeavesAverageLocation());
		if (d <= cloudRadius)
		{
			AAttractor* closest = GetClosestLeafFromBranch(LastBranch);
			float distToClosestLeaf = FVector::Dist(LastBranch->GetLocation(), closest->GetActorLocation());
			// ensure the closest has the right attraction range to attract the trunk
			// avoid any infinite loop problems or empty tree
			closest->SetAttractionDistance(distToClosestLeaf * 2.f);
			break;
		}
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
			USpaceColonizationNode* branch = Branches[j];

			float d = FVector::Dist(branch->GetLocation(), leaf->GetActorLocation());
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

			if (ActorLeafType)
			{
				FVector leafLoc = toRemove->GetActorLocation();
				FRotator leafRot = toRemove->GetActorRotation();
#if 0 // TODO : create static mesh instead of spawning a "leaf actor"
				UStaticMeshComponent* leafMesh = NewObject<UStaticMeshComponent>(UStaticMeshComponent::StaticClass(), TEXT("Static Mesh"));
				leafMesh->RegisterComponent();
				leafMesh->SetStaticMesh(LeafStaticMesh);
				leafMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
#else
				GetWorld()->SpawnActor(ActorLeafType, &leafLoc, &leafRot);
#endif
			}

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
		USpaceColonizationNode* branch = Branches[i];

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

			USpaceColonizationNode* child = branch->GrowChildNode();
			child->SetSegmentLength(SegmentLength);
			child->SetMaxThickness(MaxThickness);
			Branches.Add(child);
			OnBranchSpawnedDo(child);
		}
	}
}

void ASpaceColonizator::OnBranchSpawnedDo(USpaceColonizationNode* branch)
{
	if (bSpawnActorsOnBranches)
	{
		FVector loc = branch->GetLocation();
		FRotator rot = branch->GetRotation().Rotator();

		FActorSpawnParameters spawnParams;
		spawnParams.bNoFail = true;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* r = GetWorld()->SpawnActor(ActorBranchType, &loc, &rot, spawnParams);
		check(r);
	}

	if (bGenerateEventOnBranchSpawn && OnBranchSpawned.IsBound())
		OnBranchSpawned.Broadcast(branch);
}

// Called when the game starts or when spawned
void ASpaceColonizator::BeginPlay()
{
	Super::BeginPlay();

	LinkAttractorCloud();
	if (Leaves.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No leaves in this tree"));
		return;
	}

	// create root
	GrowRootBranch();

	// generate trunk
	GrowTrunk();


	// generate next nodes

	if (!bGrowTemporally)
	{
		while (IsAnyBranchInAnyLeafAttractionDistance())
		{
			ProcessLeaves();
			GrowBranches();
		}
		OnGrowEnd.Broadcast();
		Leaves.Empty();
	}
}

void ASpaceColonizator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bool stop = false;
	if (!stop && bGrowTemporally)
	{
		if (GrowTimerStamp >= GrowTimer)
		{
			GrowTimerStamp = 0.f;

			ProcessLeaves();
			GrowBranches();

			if (!IsAnyBranchInAnyLeafAttractionDistance())
			{
				OnGrowEnd.Broadcast();
				Leaves.Empty();
				stop = true;
			}
		}

		GrowTimerStamp += DeltaSeconds;
	}
}

void ASpaceColonizator::SetLeafCloud(AAttractorCloud* leafCloud)
{
	LeafCloud = leafCloud;
}

void ASpaceColonizator::SetGrowTemporally(bool growTemporally)
{
	this->bGrowTemporally = growTemporally;
}

void ASpaceColonizator::SetMaxThickness(float maxThickness)
{
	MaxThickness = maxThickness;
}

FVector ASpaceColonizator::GetRootLocation() const
{
	return RootBranch->GetLocation();
}

FRotator ASpaceColonizator::GetRootRotation() const
{
	return RootBranch->GetRotation().Rotator();
}
