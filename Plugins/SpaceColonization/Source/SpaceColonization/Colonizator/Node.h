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

	ANode* parent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float MaxThickness;

public:
	TArray<AAttractor*> CurrentNearbyAttractors;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;

	// Sets default values for this actor's properties
	ANode();

	void SetOptions(const float segmentLength, const float maxThickness);

	void Reset();
	ANode* GrowChildNode(const FVector& leavesAverageDirection = FVector::ZeroVector,
	                     const float segmentLengthOverride = 0.f);
	void ThickenParent();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Space Colonization")
	inline bool HasAttractors() const { return CurrentNearbyAttractors.Num() > 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Space Colonization")
	inline float GetSegmentLength() const { return SegmentLength; }
};
