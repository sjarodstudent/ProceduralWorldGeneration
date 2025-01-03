// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

class AAttractor;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class SPACECOLONIZATION_API ANode : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Arrow")
	UArrowComponent* ArrowComponent;

	float SegmentLength = 0.f;
	
public:
	// Sets default values for this actor's properties
	ANode();

	TArray<AAttractor*> CurrentNearbyAttractors;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;

	void Reset();

	inline bool HasAttractors() const { return CurrentNearbyAttractors.Num() > 0; }
	ANode* GrowChildNode(const float segmentLengthOverride = 0.f);

	inline float GetSegmentLength() const { return SegmentLength; }
	inline void SetSegmentLength(const float segmentLength) { SegmentLength = segmentLength; }
};
