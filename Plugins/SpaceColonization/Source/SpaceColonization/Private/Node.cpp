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
	// mesh default scale value
	Mesh->SetRelativeScale3D(FVector(1.5f, 0.25f, 0.25f));

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

void ANode::SetOptions(const float segmentLength, const float maxThickness)
{
	SegmentLength = segmentLength;
	
	Mesh->SetRelativeLocation(FVector(SegmentLength, 0.f, 0.f));
	FVector scale = Mesh->GetRelativeScale3D();
	Mesh->SetRelativeScale3D(FVector(SegmentLength / 100.f * 2.f, scale.Y, scale.Z));

	this->MaxThickness = maxThickness;
}



void ANode::Reset()
{
	Super::Reset();
	CurrentNearbyAttractors.Empty();
}

ANode* ANode::GrowChildNode(const float segmentLengthOverride)
{
	// spawn a new node in direction of nearby attractors

	FVector dir = FVector::Zero();
	for (int i = 0; i < CurrentNearbyAttractors.Num(); ++i)
	{
		// add direction to the attractor
		dir += CurrentNearbyAttractors[i]->GetActorLocation() - GetActorLocation();
	}
	
	// if no nearby attractors, grow based on the parent direction
	if (dir == FVector::ZeroVector)
	{
		dir = GetActorForwardVector();

#if 0
		// print spawned forward vector
		FVector fw = GetActorForwardVector();
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Root branch forward : %f, %f, %f"), fw.X, fw.Y, fw.Z));
#endif

	}
	dir.Normalize();
	FRotator rot = dir.Rotation();

	dir *= SegmentLength;

	// spawn the next node based on the computed direction
	ANode* child = GetWorld()->SpawnActor<ANode>(GetActorLocation() + dir, rot);
	child->SetOptions(SegmentLength, MaxThickness);

	// array in AACtor
	Children.Add(child);
	child->parent = this;
	
	Reset();

	child->ThickenParent();
	return child;
}

void ANode::ThickenParent()
{
	if (!parent)
		return;
	
	float Growth = 1.002f;

	FVector scale = parent->Mesh->GetRelativeScale3D();
	if (scale.Y >= parent->MaxThickness || scale.Z >= parent->MaxThickness)
		return;
	
	parent->Mesh->SetRelativeScale3D(FVector(scale.X, scale.Y * Growth, scale.Z * Growth));
	parent->ThickenParent();
}



