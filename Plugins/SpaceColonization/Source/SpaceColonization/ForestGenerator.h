// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Colonizator/AttractorCloud.h"
#include "GameFramework/Actor.h"
#include "ForestGenerator.generated.h"

class AAttractor;
class AProceduralTree;

UCLASS()
class SPACECOLONIZATION_API AForestGenerator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	float Radius = 5000.f;
	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	int TreeCount = 5;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	TSubclassOf<AAttractor> AttractorType;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	TSubclassOf<AProceduralTree> TreeType;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	float MaxGroundHeight = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	float CloudHeightRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	float CloudOffset = 75.f;
	
	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	float AttractorRadius = 450.f;

	UPROPERTY(EditAnywhere, Category = "Forest Generator")
	int AttractorCount = 50;

public:	
	// Sets default values for this actor's properties
	AForestGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
