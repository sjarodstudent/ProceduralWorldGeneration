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
	UArrowComponent* ArrowComponent;

public:	
	// Sets default values for this actor's properties
	ANode();

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	float SegmentLength;

	TArray<AAttractor*> CurrentNearbyAttractors;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;


	ANode* GenerateChildNode();
};
