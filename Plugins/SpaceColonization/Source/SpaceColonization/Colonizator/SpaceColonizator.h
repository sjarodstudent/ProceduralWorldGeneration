// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceColonizator.generated.h"

class USpaceColonizationNode;

class AAttractor;
class UArrowComponent;
class AAttractorCloud;

UCLASS()
class SPACECOLONIZATION_API ASpaceColonizator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Arrow")
	UArrowComponent* ArrowComponent;

	/**
	 * whether the trunk should follow the arrow component or and average location of all the leaves
	 */
	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	bool bTrunkFollowArrow = false;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TArray<AAttractor*> Leaves;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	AAttractorCloud* LeafCloud;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	float SegmentLength = 15.f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	bool bSpawnActorsOnBranches = false;
	
	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TSubclassOf<AActor> ActorBranchType = nullptr;

	USpaceColonizationNode* RootBranch = nullptr;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float GrowTimer = 0.1f;
	float GrowTimerStamp = 0.f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float MaxThickness = 1.25f;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	bool bGrowTemporally = true;

public:
	UPROPERTY(BlueprintReadOnly, Category="Space Colonization")
	TArray<USpaceColonizationNode*> Branches;

	// Sets default values for this actor's properties
	ASpaceColonizator();

	void LinkAttractorCloud();

	void GrowRootBranch();

	// tells if a specified branch is in a leaf's attraction distance
	bool IsBranchInAnyLeafAttractionDistance(const USpaceColonizationNode* branch) const;
	bool IsAnyBranchInAnyLeafAttractionDistance() const;

	FVector GetLeavesAverageLocation() const;
	void GrowTrunk();

	void ProcessLeaves();
	void GrowBranches();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
