// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralTree.h"

#include "ProceduralMeshComponent.h"
#include "SpaceColonization/Colonizator/Node.h"

// Sets default values
AProceduralTree::AProceduralTree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = bGrowTemporally;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
}

void AProceduralTree::VerticesMathAdd(const float f)
{
	for (FVector& v : Vertices)
	{
		v = v + f;
	}
}

void AProceduralTree::VerticesMathSub(const float f)
{
	for (FVector& v : Vertices)
	{
		v = v - f;
	}
}

void AProceduralTree::VerticesMathMultiply(const float f)
{
	for (FVector& v : Vertices)
	{
		v = v * f;
	}
}

void AProceduralTree::VerticesMathDivide(const float f)
{
	for (FVector& v : Vertices)
	{
		v = v / f;
	}
}

void AProceduralTree::VerticesRotate(const FQuat q)
{
	for (FVector& v : Vertices)
	{
		v = v.RotateAngleAxis(q.GetAngle(), q.GetRotationAxis());
	}
}

void AProceduralTree::GenerateMeshSection_Implementation(const USpaceColonizationNode* Branch)
{
}
