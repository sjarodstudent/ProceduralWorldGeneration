// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceColonizator.generated.h"

class AAttractor;
class ANode;
class UArrowComponent;
class AAttractorCloud;

UCLASS()
class SPACECOLONIZATION_API ASpaceColonizator : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Arrow")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TArray<AAttractor*> Leaves;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	AAttractorCloud* LeafCloud;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	float SegmentLength = 100.f;

	UPROPERTY(EditAnywhere, Category="Space Colonization")
	TSubclassOf<ANode> BranchType = nullptr;
	
	ANode* RootBranch = nullptr;

	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float GrowTimer = 0.1f;
	float GrowTimerStamp = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Space Colonization")
	float MaxThickness = 2.f;

public:
	
	UPROPERTY(BlueprintReadOnly, Category="Space Colonization")
	TArray<ANode*> Branches;

	
	// Sets default values for this actor's properties
	ASpaceColonizator();

	void LinkAttractorCloud();

	void GrowRootBranch();

	// tells if a specified branch is in a leaf's attraction distance
	bool IsBranchInAnyLeafAttractionDistance(const ANode* branch);
	
	void GrowTrunk();
	
	void ProcessLeaves();
	void GrowBranches();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
