// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractorCloud.h"

#include "Attractor.h"

// Sets default values
AAttractorCloud::AAttractorCloud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void AAttractorCloud::GenerateAttractors()
{
	for (int i = 0; i < AttractorCount; ++i)
	{
		const FVector selfLoc = GetActorLocation();
		const FRotator selfRot = GetActorRotation();

		FVector randomDir = FMath::VRand();
		randomDir.Normalize();
		randomDir *= FMath::FRandRange(-AttractorRadius, AttractorRadius);

		const FVector randomPos = selfLoc + randomDir;

		// TODO : random rot (maybe not useful)
		AActor* attractor = GetWorld()->SpawnActor(AttractorType, &randomPos, &selfRot);
		Attractors.Add(Cast<AAttractor>(attractor));
	}
}

// Called when the game starts or when spawned
void AAttractorCloud::BeginPlay()
{
	Super::BeginPlay();
}