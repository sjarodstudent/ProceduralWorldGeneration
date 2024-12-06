// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"

#include "Attractor.h"

// Sets default values
ANode::ANode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

// Called when the game starts or when spawned
void ANode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#define DebugLog(seconds, color, ...) GEngine->AddOnScreenDebugMessage(-1, seconds, color, FString::Printf(__VA_ARGS__))

#define DebugLogRed(...) DebugLog(15.f, FColor::Red, __VA_ARGS__)
#define DebugLogGreen(...) DebugLog(15.f, FColor::Green, __VA_ARGS__)
#define DebugLogBlue(...) DebugLog(15.f, FColor::Cyan, __VA_ARGS__)

#define DebugLogPerFrame(color, ...) DebugLog(DeltaTime, color, __VA_ARGS__)

ANode* ANode::GenerateChildNode()
{
	// spawn a new node in direction of nearby attractors
	
	FVector dir = FVector::Zero();
	for (int i = 0; i < CurrentNearbyAttractors.Num(); ++i)
	{
		dir += CurrentNearbyAttractors[i]->GetActorLocation();
	}
	dir.Normalize();

	DebugLogGreen(TEXT("%f, %f, %f"), dir.X, dir.Y, dir.Z);
	dir *= SegmentLength;
	DebugLogGreen(TEXT("%f, %f, %f"), dir.X, dir.Y, dir.Z);

	DebugLogGreen(TEXT("%f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	ANode* child = GetWorld()->SpawnActor<ANode>(GetActorLocation() + dir, FRotator());
	child->SegmentLength = SegmentLength;
	Children.Add(child);
	return child;
}