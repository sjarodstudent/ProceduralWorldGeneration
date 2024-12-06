// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attractor.generated.h"

class ANode;

UCLASS()
class SPACECOLONIZATION_API AAttractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttractor();

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float AttractionDistance;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float KillDistance;

	ANode* CurrentAttractedNode = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
