// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseTerrainGenerator.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;
UCLASS()
class PWG_API APerlinNoiseTerrainGenerator : public AActor
{
    GENERATED_BODY()

public:
    APerlinNoiseTerrainGenerator();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    int32 Width = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    int32 Height = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    int32 Seed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    float CellSize = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    float HeightMultiplier = 1000.0f;

    int TerrainIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perlin Noise")
    UMaterial* ParentMaterial = nullptr;

    UProceduralMeshComponent* ProceduralMesh;

    TArray<UProceduralMeshComponent*> ProceduralMeshArray;

    UTexture2D* GeneratePerlinNoiseTexture();

    UTexture2D* GeneratePerlinNoiseTexture(int32 StartX, int32 StartY);

    UFUNCTION(CallInEditor, Category = "Perlin Noise")
    void UpdatePerlinNoiseTexture();

    void GenerateTerrain(int32 StartX, int32 StartY);

    void GenerateNeighboorTerrain();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UMaterialInstanceDynamic* MyMaterial;

    UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
    TArray<FVector> Vertices;

    UPROPERTY(EditAnywhere)
    TArray<int> Triangles;

    UPROPERTY(EditAnywhere)
    TArray<FVector2D> UV0;

    TArray<TArray<FVector>> VerticesArray;
    TArray<TArray<int>> TrianglesArray;
    TArray<TArray<FVector2D>> UVArray;

    UMaterialInstanceDynamic* MyMaterialPerlinNoise;
};
