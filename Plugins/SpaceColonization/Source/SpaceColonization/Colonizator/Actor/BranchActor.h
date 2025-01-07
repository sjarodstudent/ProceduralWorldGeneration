// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BranchActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SPACECOLONIZATION_API ABranchActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;
	
	float SegmentLength;
	float MaxThickness;

	// Sets default values for this actor's properties
	ABranchActor();
	
	void SetOptions(float segmentLength, float maxThickness);
};
