// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements")
    int CastleCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements")
    float Radius = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements")
    float MaxSlope = 0.1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements")
    TSubclassOf<AActor> CastleBP;

    UProceduralMeshComponent* ProceduralMesh;

    TArray<UProceduralMeshComponent*> ProceduralMeshArray;

    UTexture2D* GeneratePerlinNoiseTexture();

    UTexture2D* GeneratePerlinNoiseTexture(int32 StartX, int32 StartY);

    UFUNCTION(CallInEditor, Category = "Perlin Noise")
    void UpdatePerlinNoiseTexture();

    void GenerateTerrain(int StartX, int StartY);

    void GenerateNeighboorTerrain();

    void GenerateCastlePoint(int StartX, int StartY);

    int GetRandomValue();

    bool IsAreaFlat(int32 X, int32 Y);

    void GenerateTreePoint();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    int SpawnXCastleLimit = Width - 50;
    int SpawnYCastleLimit = Height - 50;

    UStaticMeshComponent* SphereMesh;

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

    FVector CastleLocation;
};
