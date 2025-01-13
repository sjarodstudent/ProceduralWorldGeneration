// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseTerrainGenerator.generated.h"

class ASpaceColonizator;
class AAttractor;
class UProceduralMeshComponent;
class UMaterialInterface;
class AForestGenerator;

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
    float Radius = 10000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Castle")
    int CastleCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Castle")
    TSubclassOf<AActor> CastleBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Forest")
    int ForestCount = 10;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Forest")
    TSubclassOf<AForestGenerator> ForestBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Road")
    bool bSpawnRoad;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Road")
    TSubclassOf<AAttractor> RoadAttractorBP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Elements Road")
    TSubclassOf<ASpaceColonizator> RoadBP;
    ASpaceColonizator* RoadColonizatorHandle;

    UProceduralMeshComponent* ProceduralMesh;

    TArray<UProceduralMeshComponent*> ProceduralMeshArray;

    UTexture2D* GeneratePerlinNoiseTexture();

    UTexture2D* GeneratePerlinNoiseTexture(int StartX, int StartY);

    void GenerateTerrain(int StartX, int StartY);

    void GenerateNeighboorTerrain();

    void GenerateCastlePoint(int StartX, int StartY);

    int GetRandomValue();

    void GenerateForestPoints(int StartX, int StartY);

    TArray<FVector> ForestPoints;

    bool IsPointValidForForest(const FVector& Point, float MinDistance);

    void LerpPixelColor(int x, int y, float NormalizedHeight, uint8* Data);

    void GeneratePoints(int StartX, int StartY, TArray<FVector>* Points);

protected:
    virtual void BeginPlay() override;

    int SpawnXCastleLimit = Width - 50;
    int SpawnYCastleLimit = Height - 50;

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

    TArray<FVector> CastleLocations;
};
