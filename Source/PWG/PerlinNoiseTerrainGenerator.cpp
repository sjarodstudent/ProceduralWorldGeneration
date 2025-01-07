// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseTerrainGenerator.h"

#include "Kismet/GameplayStatics.h"

APerlinNoiseTerrainGenerator::APerlinNoiseTerrainGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APerlinNoiseTerrainGenerator::BeginPlay()
{
    Super::BeginPlay();

    GenerateNeighboorTerrain();
}

void APerlinNoiseTerrainGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //UpdateLoadedTerrains();
}

void APerlinNoiseTerrainGenerator::GenerateTerrain(int32 StartX, int32 StartY)
{
    TArray<FVector> tempVertices; TArray<int> tempTriangles; TArray<FVector2D> tempUV;

    tempVertices.Empty();
    tempTriangles.Empty();
    tempUV.Empty();

    VerticesArray.Insert(tempVertices, VerticesArray.Num());
    TrianglesArray.Insert(tempTriangles, TrianglesArray.Num());
    UVArray.Insert(tempUV, UVArray.Num());

    for (int32 y = 0; y <= Height; ++y)
    {
        for (int32 x = 0; x <= Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float HeightValue = NoiseValue * HeightMultiplier;

            VerticesArray[VerticesArray.Num() - 1].Add(FVector((StartX + x) * CellSize, (StartY + y) * CellSize, HeightValue));
            UVArray[UVArray.Num() - 1].Add(FVector2D((float)x / Width, (float)y / Height));
        }
    }

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            int32 Index1 = x + (y * (Width + 1));
            int32 Index2 = Index1 + 1;
            int32 Index3 = Index1 + (Width + 1);
            int32 Index4 = Index3 + 1;

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
        FString MeshName = FString::Printf(TEXT("ProceduralMesh%d"), TerrainIndex);
        proceduralMesh->RegisterComponentWithWorld(GetWorld()); // Enregistre le composant dans le monde
        proceduralMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // Attache le composant au RootComponent

        proceduralMesh->SetWorldLocation(FVector(0, 0, 0));
        proceduralMesh->SetWorldScale3D(FVector(1, 1, 1));

        ProceduralMeshArray.Add(proceduralMesh);
    }
    else
        UE_LOG(LogTemp, Error, TEXT("Failed to create UProceduralMeshComponent"));

    if (TerrainIndex < ProceduralMeshArray.Num())
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
    // Générer le terrain principal
    GenerateTerrain(0, 0);

    // Générer les terrains voisins
    GenerateTerrain(Width, 0); // Terrain à droite
    GenerateTerrain(-Width, 0); // Terrain à gauche
    GenerateTerrain(0, Height); // Terrain en haut
    GenerateTerrain(0, -Height); // Terrain en bas
    GenerateTerrain(Width, Height);
    GenerateTerrain(-Width, Height);
    GenerateTerrain(Width, -Height);
    GenerateTerrain(-Width, -Height);
}

void APerlinNoiseTerrainGenerator::UpdatePerlinNoiseTexture()
{
    UTexture2D* PerlinTexture = GeneratePerlinNoiseTexture();

    if (PerlinTexture)
    {
        MyMaterialPerlinNoise = UMaterialInstanceDynamic::Create(ParentMaterial, NULL);
        MyMaterialPerlinNoise->SetTextureParameterValue(TEXT("PerlinTextureSample"), PerlinTexture);

        ProceduralMeshArray[0]->SetMaterial(0, MyMaterialPerlinNoise);
        ProceduralMeshArray[0]->SetCastShadow(false);
    }
}

UTexture2D* APerlinNoiseTerrainGenerator::GeneratePerlinNoiseTexture(int32 StartX, int32 StartY)
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

    float WaterHeight = 0.3f;
    float GrassHeight = 0.8f;

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + StartX + Seed) / CellSize, (y + StartY + Seed) / CellSize));
            float NormalizedHeight = (NoiseValue + 1.0f) * 0.5f;

            uint8 R = 0, G = 0, B = 0;

            /*if (NormalizedHeight < WaterHeight)
            {
                R = 255;
                G = 0;
                B = 0;
            }
            else if (NormalizedHeight < GrassHeight)
            {
                R = 34;
                G = 139;
                B = 34;
            }
            else
            {
                R = 255;
                G = 255;
                B = 255;
            }*/

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

            int32 PixelIndex = (y * Width + x) * 4;
            Data[PixelIndex + 0] = R;
            Data[PixelIndex + 1] = G;
            Data[PixelIndex + 2] = B;
            Data[PixelIndex + 3] = 255;
        }
    }

    RawData.Unlock();
    PerlinTexture->UpdateResource();

    return PerlinTexture;
}

UTexture2D* APerlinNoiseTerrainGenerator::GeneratePerlinNoiseTexture()
{
    UTexture2D* PerlinTexture = UTexture2D::CreateTransient(Width, Height);
    if (!PerlinTexture)
        return nullptr;

    //PerlinTexture->GetPlatformData()->Mips.SetNum(1);
    FTexture2DMipMap& Mip = PerlinTexture->GetPlatformData()->Mips[0];
    Mip.SizeX = Width;
    Mip.SizeY = Height;

    FByteBulkData& RawData = Mip.BulkData;
    RawData.Lock(LOCK_READ_WRITE);
    uint8* Data = (uint8*)RawData.Realloc(Width * Height * 4); // RGBA

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            float NoiseValue = FMath::PerlinNoise2D(FVector2D((x + Seed) / CellSize, (y + Seed) / CellSize));

            uint8 ColorValue = FMath::Clamp((NoiseValue + 1.0f) * 127.5f, 0.0f, 255.0f);

            int32 PixelIndex = (y * Width + x) * 4;
            Data[PixelIndex + 0] = ColorValue;
            Data[PixelIndex + 1] = ColorValue;
            Data[PixelIndex + 2] = ColorValue;
            Data[PixelIndex + 3] = 255;
        }
    }

    // Need it for modifying data
    RawData.Unlock();

    PerlinTexture->UpdateResource();

    return PerlinTexture;
}