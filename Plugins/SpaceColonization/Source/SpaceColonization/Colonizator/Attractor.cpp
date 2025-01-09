// Fill out your copyright notice in the Description page of Project Settings.


#include "Attractor.h"

#include "Node.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAttractor::AAttractor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

#if 0
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	Mesh->SetStaticMesh(SphereMesh.Object);
#endif
}

void AAttractor::Reset()
{
	Super::Reset();
	CurrentAttractedNode = nullptr;
}

float AAttractor::GetDistanceToCurrentAttractedNode() const
{
	if (!CurrentAttractedNode)
		return -1.f;

	return GetDistanceToBranch(CurrentAttractedNode);
}

bool AAttractor::IsInAttractionRange() const
{
	return GetDistanceToCurrentAttractedNode() <= AttractionDistance &&
		GetDistanceToCurrentAttractedNode() > KillDistance;
}

bool AAttractor::IsReached() const
{
	// is not reached since it is not attracted
	if (GetDistanceToCurrentAttractedNode() < 0.f)
		return false;

	return GetDistanceToCurrentAttractedNode() <= KillDistance;
}

float AAttractor::GetDistanceToBranch(const USpaceColonizationNode* branch) const
{
	FVector branchLoc = branch->GetLocation();
	FVector selfLoc = GetActorLocation();
	FVector dir = branchLoc - selfLoc;
	return dir.Length();
}

void AAttractor::DrawDebug()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttractionDistance, 30, FColor::Yellow, true, 999.f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), KillDistance, 30, FColor::Red, true, 999.f);
}

void AAttractor::BeginPlay()
{
	Super::BeginPlay();

	if (bDrawDebug)
	{
		DrawDebug();
	}
}
