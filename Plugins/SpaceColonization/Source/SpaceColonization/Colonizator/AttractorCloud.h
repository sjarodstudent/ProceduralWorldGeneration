// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttractorCloud.generated.h"

class AAttractor;

UCLASS()
class SPACECOLONIZATION_API AAttractorCloud : public AActor
{
	GENERATED_BODY()

private:
	// generated attractors
	TArray<AAttractor*> Attractors;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Space Colonization")
	int AttractorCount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Space Colonization")
	float AttractorRadius = 1000.f;

public:
	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	TSubclassOf<AAttractor> AttractorType = nullptr;

	// Sets default values for this actor's properties
	AAttractorCloud();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void GenerateAttractors();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Space Colonization")
	inline TArray<AAttractor*> GetAttractorArray() const
	{
		return Attractors;
	}
};
