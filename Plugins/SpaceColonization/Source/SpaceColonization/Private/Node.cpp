// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"

#include "Attractor.h"
#include "Components/ArrowComponent.h"

// Sets default values
ANode::ANode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	Mesh->SetStaticMesh(CubeMesh.Object);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
		ArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA
}


ANode* ANode::GenerateChildNode()
{
	// spawn a new node in direction of nearby attractors

	FVector dir = FVector::Zero();
	for (int i = 0; i < CurrentNearbyAttractors.Num(); ++i)
	{
		dir += CurrentNearbyAttractors[i]->GetActorLocation();
	}
	if (dir == FVector::ZeroVector)
		dir = GetActorForwardVector();
	dir.Normalize();

	dir *= SegmentLength;

	ANode* child = GetWorld()->SpawnActor<ANode>(GetActorLocation() + dir, FRotator());
	child->SegmentLength = SegmentLength;
	Children.Add(child);
	return child;
}