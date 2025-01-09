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

TArray<FVector> AProceduralTree::VerticesAdd(const TArray<FVector>& inputVertex, const float f)
{
	TArray<FVector> newVertices;
	for (const FVector& v : inputVertex)
	{
		newVertices.Add(v + f);
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VerticesSub(const TArray<FVector>& inputVertex, const float f)
{
	TArray<FVector> newVertices;
	for (const FVector& v : inputVertex)
	{
		newVertices.Add(v - f);
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VerticesSubVector(const TArray<FVector>& inputVertex, const FVector& v)
{
	TArray<FVector> newVertices;
	for (const FVector& vv : inputVertex)
	{
		newVertices.Add((vv - v));
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VectorSubVertices(const TArray<FVector>& inputVertex, const FVector& v)
{
	TArray<FVector> newVertices;
	for (const FVector& vv : inputVertex)
	{
		newVertices.Add(v - vv);
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VerticesMultiply(const TArray<FVector>& inputVertex, const float f)
{
	TArray<FVector> newVertices;
	for (const FVector& v : inputVertex)
	{
		newVertices.Add(v * f);
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VerticesDivide(const TArray<FVector>& inputVertex, const float f)
{
	TArray<FVector> newVertices;
	for (const FVector& v : inputVertex)
	{
		newVertices.Add(v / f);
	}
	return newVertices;
}

TArray<FVector> AProceduralTree::VerticesRotate(const TArray<FVector>& inputVertex, const FQuat q)
{
	TArray<FVector> newVertices;
	for (const FVector& v : inputVertex)
	{
		newVertices.Add(q.GetNormalized() * v);
	}
	return newVertices;
}

void AProceduralTree::GenerateMeshSection_Implementation(const USpaceColonizationNode* Branch)
{
}
