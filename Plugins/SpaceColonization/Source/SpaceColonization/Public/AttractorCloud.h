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
	// Sets default values for this actor's properties
	AAttractorCloud();

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	int AttractorCount = 10;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float AttractorRadius = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	TSubclassOf<AAttractor> AttractorType = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateAttractors();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Space Colonization")
	TArray<AAttractor*> GetAttractorArray() const;

};

inline TArray<AAttractor*> AAttractorCloud::GetAttractorArray() const
{
	return Attractors;
}

