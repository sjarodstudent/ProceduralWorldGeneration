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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TArray<int> Triangles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	UProceduralMeshComponent* ProceduralMesh;
	
public:	
	// Sets default values for this actor's properties
	AProceduralTree();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesAdd(const TArray<FVector>& inputVertex, const float f);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesSub(const TArray<FVector>& inputVertex, const float f);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesSubVector(const TArray<FVector>& inputVertex, const FVector& v);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VectorSubVertices(const TArray<FVector>& inputVertex, const FVector& v);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesMultiply(const TArray<FVector>& inputVertex, const float f);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesDivide(const TArray<FVector>& inputVertex, const float f);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> VerticesRotate(const TArray<FVector>& inputVertex, const FQuat q);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Space Colonization")
	void GenerateMeshSection(const USpaceColonizationNode* Branch);
};
