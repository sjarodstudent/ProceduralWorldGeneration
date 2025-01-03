// Fill out your copyright notice in the Description page of Project Settings.


#include "Attractor.h"

#include "Node.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAttractor::AAttractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	Mesh->SetStaticMesh(SphereMesh.Object);
}

void AAttractor::Reset()
{
	Super::Reset();
	CurrentAttractedNode = nullptr;
}

float AAttractor::GetDistanceToCurrentAttractedNode()
{
	if (!CurrentAttractedNode)
		return -1.f;

	return GetDistanceTo(CurrentAttractedNode);
}

bool AAttractor::IsInAttractionRange()
{
	return GetDistanceToCurrentAttractedNode() <= AttractionDistance &&
		GetDistanceToCurrentAttractedNode() > KillDistance;
}

bool AAttractor::IsReached()
{
	// is not reached since it is not attracted
	if (GetDistanceToCurrentAttractedNode() < 0.f)
		return false;
	
	return GetDistanceToCurrentAttractedNode() <= KillDistance;
}

void AAttractor::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), AttractionDistance, 30, FColor::Yellow, true, 999.f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), KillDistance, 30, FColor::Red, true, 999.f);
}
