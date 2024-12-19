// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractor.generated.h"

class ANode;
class UStaticMeshComponent;

UCLASS()
class SPACECOLONIZATION_API AAttractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttractor();
	float GetDistanceToCurrentAttractedNode();
	bool IsInAttractionRange();
	bool IsReached();

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float AttractionDistance;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float KillDistance;

	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;

	ANode* CurrentAttractedNode = nullptr;

	
};
