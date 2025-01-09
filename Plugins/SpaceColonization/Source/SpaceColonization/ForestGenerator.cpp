// Fill out your copyright notice in the Description page of Project Settings.


#include "ForestGenerator.h"

#include "Colonizator/Attractor.h"
#include "Colonizator/AttractorCloud.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceColonization/Mesh/ProceduralTree.h"
#include "Kismet/KismetMathLibrary.h"

#include "Mesh/ProceduralTree.h"

// Sets default values
AForestGenerator::AForestGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AForestGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < TreeCount; ++i)
	{
		FVector randomDir = FMath::VRand();
		// erase depth
		randomDir.Z = 0.f;
		randomDir.Normalize();
		float randomDistance = FMath::FRand() * Radius;
		randomDir *= randomDistance;

		// start of raycast (in height)
		FVector start = FVector(randomDir.X, randomDir.Y, MaxGroundHeight);
		FVector end = start + FVector::DownVector * MaxGroundHeight * 2.f;
		FHitResult hit;
		bool bFound = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_WorldStatic);
		if (!bFound)
			continue;

		FVector treeLocation = hit.Location;
		FRotator treeRotation = FRotator(0.f, FMath::FRand() * 360.f, 0.f);
		FTransform treeTransform;
		treeTransform.SetLocation(treeLocation);
		treeTransform.SetRotation(treeRotation.Quaternion());
		AActor* treeActor = GetWorld()->SpawnActorDeferred<AProceduralTree>(TreeType, treeTransform);
		AProceduralTree* tree = Cast<AProceduralTree>(treeActor);
		
		FVector cloudLocation = treeLocation + FVector::UpVector * FMath::RandRange(0.6f, 1.f) * CloudHeightRange;
		FVector randCloudOffset = FMath::VRand();
		randCloudOffset.Normalize();
		randCloudOffset *= FMath::FRand() * CloudOffset;
		FRotator cloudRotation = UKismetMathLibrary::RandomRotator();

		FTransform cloudTransform;
		cloudTransform.SetLocation(cloudLocation);
		cloudTransform.SetRotation(cloudRotation.Quaternion());
		AActor* cloudActor = GetWorld()->SpawnActorDeferred<AAttractorCloud>(AAttractorCloud::StaticClass(), cloudTransform);
		AAttractorCloud* cloud = Cast<AAttractorCloud>(cloudActor);

		cloud->AttractorType = AttractorType;
		cloud->AttractorRadius = AttractorRadius;
		cloud->AttractorCount = AttractorCount;
		UGameplayStatics::FinishSpawningActor(cloudActor, cloudTransform);

		tree->SetActorLeafType(ActorLeafType);
		tree->SetLeafCloud(cloud);
		tree->SetGrowTemporally(bGrowTemporally);
		tree->SetMaxThickness(TreeMaxThickness);
		UGameplayStatics::FinishSpawningActor(treeActor, treeTransform);
	}
}
