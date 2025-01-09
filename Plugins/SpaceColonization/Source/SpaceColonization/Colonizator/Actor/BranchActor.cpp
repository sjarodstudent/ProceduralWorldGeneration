// Fill out your copyright notice in the Description page of Project Settings.

#include "BranchActor.h"

// Sets default values
ABranchActor::ABranchActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	Mesh->SetStaticMesh(CubeMesh.Object);
	// mesh default scale value
	Mesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
}

void ABranchActor::SetOptions(const float segmentLength, const float maxThickness)
{
	SegmentLength = segmentLength;

	Mesh->SetRelativeLocation(FVector(SegmentLength, 0.f, 0.f));
	FVector scale = Mesh->GetRelativeScale3D();
	Mesh->SetRelativeScale3D(FVector(SegmentLength / 100.f * 2.f, scale.Y, scale.Z));

	this->MaxThickness = maxThickness;
}