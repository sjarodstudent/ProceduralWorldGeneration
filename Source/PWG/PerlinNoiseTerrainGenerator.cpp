// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoiseTerrainGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "SpaceColonization/Colonizator/SpaceColonizator.h"
#include "SpaceColonization/Colonizator/Attractor.h"
#include "SpaceColonization/ForestGenerator.h"

APerlinNoiseTerrainGenerator::APerlinNoiseTerrainGenerator()
{
}

int APerlinNoiseTerrainGenerator::GetRandomValue()
{
    TArray<int> possibleValues = { Width, Height, -Width, -Height, 0 };

    int randomIndex = FMath::RandRange(0, possibleValues.Num() - 1);

    return possibleValues[randomIndex];
}

void APerlinNoiseTerrainGenerator::BeginPlay()
{
    Super::BeginPlay();

    GenerateNeighboorTerrain();

    if (bSpawnRoad)
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(GetActorLocation());
        SpawnTransform.SetRotation(GetActorRotation().Quaternion());
        RoadColonizatorHandle = GetWorld()->SpawnActorDeferred<ASpaceColonizator>(RoadBP, SpawnTransform);
    }
    
    for(int i = 0; i < CastleCount; i++)
        GenerateCastlePoint(GetRandomValue(), GetRandomValue());
    
    for (int i = 0; i < ForestCount; i++)
        GenerateForestPoints(GetRandomValue(), GetRandomValue());

    if (bSpawnRoad)
        RoadColonizatorHandle->FinishSpawning(RoadColonizatorHandle->GetTransform());
}

void APerlinNoiseTerrainGenerator::GenerateTerrain(int StartX, int StartY)
{
    TArray<FVector> tempVertices; TArray<int> tempTriangles; TArray<FVector2D> tempUV;

    tempVertices.Empty();
    tempTriangles.Empty();
    tempUV.Empty();

    VerticesArray.Insert(tempVertices, VerticesArray.Num());
    TrianglesArray.Insert(tempTriangles, TrianglesArray.Num());
    UVArray.Insert(tempUV, UVArray.Num());

    for (int y = 0; y <= Height; ++y)
    {
        for (int x = 0; x <= Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float HeightValue = NoiseValue * HeightMultiplier;

            VerticesArray[VerticesArray.Num() - 1].Add(FVector((StartX + x) * CellSize, (StartY + y) * CellSize, HeightValue));
            UVArray[UVArray.Num() - 1].Add(FVector2D((float)x / Width, (float)y / Height));
        }
    }

    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            int Index1 = x + (y * (Width + 1));
            int Index2 = Index1 + 1;
            int Index3 = Index1 + (Width + 1);
            int Index4 = Index3 + 1;

            TrianglesArray[TrianglesArray.Num() - 1].Add(Index1);
            TrianglesArray[TrianglesArray.Num() - 1].Add(Index3);
            TrianglesArray[TrianglesArray.Num() - 1].Add(Index2);

            TrianglesArray[TrianglesArray.Num() - 1].Add(Index2);
            TrianglesArray[TrianglesArray.Num() - 1].Add(Index3);
            TrianglesArray[TrianglesArray.Num() - 1].Add(Index4);
        }
    }

    UProceduralMeshComponent* proceduralMesh = NewObject<UProceduralMeshComponent>(this);

    if (proceduralMesh)
    {
        proceduralMesh->RegisterComponentWithWorld(GetWorld());
        proceduralMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

        proceduralMesh->SetWorldLocation(FVector(0, 0, 0));
        proceduralMesh->SetWorldScale3D(FVector(1, 1, 1));

        ProceduralMeshArray.Add(proceduralMesh);
    }

    ProceduralMeshArray[TerrainIndex]->CreateMeshSection(0, VerticesArray[VerticesArray.Num() - 1], TrianglesArray[TrianglesArray.Num() - 1], TArray<FVector>(), UVArray[UVArray.Num() - 1], TArray<FColor>(), TArray<FProcMeshTangent>(), true);

    UTexture2D* PerlinTexture = GeneratePerlinNoiseTexture(StartX, StartY);

    if (PerlinTexture)
    {
        MyMaterialPerlinNoise = UMaterialInstanceDynamic::Create(ParentMaterial, NULL);
        MyMaterialPerlinNoise->SetTextureParameterValue(TEXT("PerlinTextureSample"), PerlinTexture);

        if (TerrainIndex < ProceduralMeshArray.Num())
        {
            ProceduralMeshArray[TerrainIndex]->SetMaterial(0, MyMaterialPerlinNoise);
            ProceduralMeshArray[TerrainIndex]->SetCastShadow(false);
        }
    }

    TerrainIndex += 1;
}

void APerlinNoiseTerrainGenerator::GenerateNeighboorTerrain()
{
    GenerateTerrain(0, 0);

    GenerateTerrain(Width, 0);
    GenerateTerrain(-Width, 0);
    GenerateTerrain(0, Height);
    GenerateTerrain(0, -Height);
    GenerateTerrain(Width, Height);
    GenerateTerrain(-Width, Height);
    GenerateTerrain(Width, -Height);
    GenerateTerrain(-Width, -Height);
}

UTexture2D* APerlinNoiseTerrainGenerator::GeneratePerlinNoiseTexture(int StartX, int StartY)
{
    UTexture2D* PerlinTexture = UTexture2D::CreateTransient(Width, Height);

    if (!PerlinTexture)
        return nullptr;

    FTexture2DMipMap& Mip = PerlinTexture->GetPlatformData()->Mips[0];
    Mip.SizeX = Width;
    Mip.SizeY = Height;

    FByteBulkData& RawData = Mip.BulkData;
    RawData.Lock(LOCK_READ_WRITE);
    uint8* Data = (uint8*)RawData.Realloc(Width * Height * 4);

    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float NormalizedHeight = (NoiseValue + 1.0f) * 0.5f;

            LerpPixelColor(x, y, NormalizedHeight, Data);
        }
    }

    RawData.Unlock();
    PerlinTexture->UpdateResource();

    return PerlinTexture;
}

void APerlinNoiseTerrainGenerator::LerpPixelColor(int x, int y, float NormalizedHeight, uint8* Data)
{
    float WaterHeight = 0.3f;
    float GrassHeight = 0.8f;

    int R = 0, G = 0, B = 0;

    if (NormalizedHeight < WaterHeight)
    {
        float Factor = NormalizedHeight / WaterHeight;
        R = FMath::Lerp(150, 255, Factor);
        G = FMath::Lerp(0, 130, Factor);
        B = FMath::Lerp(0, 70, Factor);
    }
    else if (NormalizedHeight < GrassHeight)
    {
        float Factor = (NormalizedHeight - WaterHeight) / (GrassHeight - WaterHeight);
        R = FMath::Lerp(34, 85, Factor);
        G = FMath::Lerp(139, 255, Factor);
        B = FMath::Lerp(34, 85, Factor);
    }
    else
    {
        float Factor = (NormalizedHeight - GrassHeight) / (1.0f - GrassHeight);
        R = FMath::Lerp(200, 255, Factor);
        G = FMath::Lerp(200, 255, Factor);
        B = FMath::Lerp(200, 255, Factor);
    }

    int PixelIndex = (y * Width + x) * 4;
    Data[PixelIndex + 0] = R;
    Data[PixelIndex + 1] = G;
    Data[PixelIndex + 2] = B;
    Data[PixelIndex + 3] = 255;
}

void APerlinNoiseTerrainGenerator::GenerateCastlePoint(int StartX, int StartY)
{
    TArray<FVector> GrassLocations;

    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float NormalizedHeight = (NoiseValue + 1.0f) * 0.5f;

            if (NormalizedHeight >= 0.3f && NormalizedHeight < 0.8f)
                GrassLocations.Add(FVector((StartX + x) * CellSize, (StartY + y) * CellSize, HeightMultiplier));
        }
    }

    if (GrassLocations.Num() > 0)
    {
        int RandomIndex = FMath::RandRange(0, GrassLocations.Num() - 1);
        CastleLocations.Add(GrassLocations[RandomIndex]);
        GetWorld()->SpawnActor<AActor>(CastleBP, GrassLocations[RandomIndex], FRotator(0, 0, 0));

        if (bSpawnRoad)
        {
            FVector SpawnLocation = GrassLocations[RandomIndex];
            FRotator SpawnRotation = FRotator(0, 0, 0);
            AAttractor* roadAttractor = Cast<AAttractor>(GetWorld()->SpawnActor(RoadAttractorBP, &SpawnLocation, &SpawnRotation));
            roadAttractor->SetAttractionDistance(10000);

            RoadColonizatorHandle->AddLeaf(roadAttractor);
        }
    }
}

void APerlinNoiseTerrainGenerator::GenerateForestPoints(int StartX, int StartY)
{
    TArray<FVector> Forests;

    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float NormalizedHeight = (NoiseValue + 1.0f) * 0.5f;

            if (NormalizedHeight >= 0.3f && NormalizedHeight < 0.8f)
                Forests.Add(FVector((StartX + x) * CellSize, (StartY + y) * CellSize, HeightMultiplier));
        }
    }

    if (Forests.Num() > 0)
    {
        for (int i = 0; i < 100; i++)
        {
            int RandomIndex = FMath::RandRange(0, Forests.Num() - 1);

            if (IsPointValidForForest(Forests[RandomIndex], Radius))
            {
                ForestPoints.Add(Forests[RandomIndex]);

                FVector SpawnLocation = Forests[RandomIndex];
                FRotator SpawnRotation = FRotator(0, 0, 0);
                GetWorld()->SpawnActor(ForestBP, &SpawnLocation, &SpawnRotation);

                DrawDebugSphere(GetWorld(), Forests[RandomIndex], 5000.f, 10, FColor::Red, true);

                break;
            }
        }
    }
}

bool APerlinNoiseTerrainGenerator::IsPointValidForForest(const FVector& Point, float MinDistance)
{
    for (const FVector& CastleLocation : CastleLocations)
    {
        float test = FVector::Dist(Point, CastleLocation);

        if (test < MinDistance)
            return false;
    }

    for (const FVector& ForestPoint : ForestPoints)
    {
        if (FVector::Dist(Point, ForestPoint) < MinDistance)
            return false;
    }

    return true;
}
