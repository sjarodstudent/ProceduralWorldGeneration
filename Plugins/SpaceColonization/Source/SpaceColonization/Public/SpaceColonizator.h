// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceColonizator.generated.h"

class AAttractor;
class ANode;

UCLASS()
class SPACECOLONIZATION_API ASpaceColonizator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TArray<AAttractor*> Leaves;
	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TArray<ANode*> Branches;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	float SegmentLength;

	
	ANode* RootBranch = nullptr;
	
public:	
	// Sets default values for this actor's properties
	ASpaceColonizator();
	bool IsAnyBranchInAttractionDistance();
	void GenerateTrunk();
	void ProcessLeaves();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
