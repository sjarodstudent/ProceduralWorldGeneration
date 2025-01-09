// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Colonizator/SpaceColonizator.h"
#include "ProceduralTree.generated.h"

class UProceduralMeshComponent;
class USpaceColonizationNode;

UCLASS()
class SPACECOLONIZATION_API AProceduralTree : public ASpaceColonizator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Procedural Mesh")
	TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, Category = "Procedural Mesh")
	TArray<int> Triangles;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	UProceduralMeshComponent* ProceduralMesh;
	
public:	
	// Sets default values for this actor's properties
	AProceduralTree();

	UFUNCTION(BlueprintCallable)
	void VerticesMathAdd(const float f);
	UFUNCTION(BlueprintCallable)
	void VerticesMathSub(const float f);
	UFUNCTION(BlueprintCallable)
	void VerticesMathMultiply(const float f);
	UFUNCTION(BlueprintCallable)
	void VerticesMathDivide(const float f);
	UFUNCTION(BlueprintCallable)
	void VerticesRotate(const FQuat q);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Space Colonization")
	void GenerateMeshSection(const USpaceColonizationNode* Branch);
};
