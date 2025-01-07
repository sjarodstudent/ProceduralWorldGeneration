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

private:
	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float AttractionDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float KillDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	bool bDrawDebug = false;

#if 0
	UPROPERTY(EditAnywhere, Category = "Rendering")
	UStaticMeshComponent* Mesh;
#endif

public:
	ANode* CurrentAttractedNode = nullptr;

	// Sets default values for this actor's properties
	AAttractor();

	void Reset();

	UFUNCTION(BlueprintCallable, Category = "Space Colonization")
	float GetDistanceToCurrentAttractedNode() const;
	UFUNCTION(BlueprintCallable, Category = "Space Colonization")
	bool IsInAttractionRange() const;
	UFUNCTION(BlueprintCallable, Category = "Space Colonization")
	bool IsReached() const;

	UFUNCTION(BlueprintCallable, Category = "Space Colonization")
	void DrawDebug();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Space Colonization")
	inline float GetAttractionDistance() const { return AttractionDistance; }
};
