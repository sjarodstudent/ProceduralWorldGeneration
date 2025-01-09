// Fill out your copyright notice in the Description page of Project Settings.


#include "CastleGenerator.h"
#include "Engine/StaticMeshActor.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACastleGenerator::ACastleGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACastleGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateSize();
	GenerateOutWalls();
	GeneratePerlinNoiseDungeon();
	GenerateCenter(); 
}

// Called every frame
void ACastleGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACastleGenerator::GenerateOutWalls()
{
	// Front
	for (int i = 0; i < OutWidth; i++)
	{
		float X = CellSize * OutDepth + CellSize;
		float Y = ((CellSize * 2) * i) - (CellSize * (OutWidth - 1));
		float Z = 0;
		float rotZ = 180;

		if (OutWalls.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutWalls.Num() - 1);
			UStaticMesh* RandomMesh = OutWalls[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutWalls is empty"));
		}
	}

	// Back
	for (int i = 0; i < OutWidth; i++)
	{
		float X = ((CellSize * OutDepth * -1 * 2) + CellSize * OutDepth) - CellSize;
		float Y = ((CellSize * 2) * i) - (CellSize * (OutWidth - 1));
		float Z = 0;
		float rotZ = 0;

		if (OutWalls.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutWalls.Num() - 1);
			UStaticMesh* RandomMesh = OutWalls[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutWalls is empty"));
		}
	}

	// Right
	for (int i = 0; i < OutDepth; i++)
	{
		float X = ((CellSize * 2) * i) - (CellSize * (OutDepth - 1));
		float Y = ((CellSize * OutWidth * -1 * 2) + CellSize * OutWidth) - CellSize;
		float Z = 0;
		float rotZ = 90;

		if (OutWalls.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutWalls.Num() - 1);
			UStaticMesh* RandomMesh = OutWalls[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutWalls is empty"));
		}
	}

	// Left
	for (int i = 0; i < OutDepth; i++)
	{
		float X = ((CellSize * 2) * i) - (CellSize * (OutDepth - 1));
		float Y = CellSize * OutWidth + CellSize;
		float Z = 0;
		float rotZ = 270;

		if (OutWalls.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutWalls.Num() - 1);
			UStaticMesh* RandomMesh = OutWalls[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutWalls is empty"));
		}
	}

	// Corner 1
	{
		float X = CellSize + (CellSize * OutDepth);
		float Y = (CellSize * 2 * (OutWidth - 1)) - ((OutWidth - 1) * CellSize) + CellSize * 2;
		float Z = 0;
		float rotZ = 90;

		if (OutCorners.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutCorners.Num() - 1);
			UStaticMesh* RandomMesh = OutCorners[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutCorners is empty"));
		}
	}

	// Corner 2
	{
		float X = CellSize + (CellSize * OutDepth);
		float Y = (0 - CellSize * (OutWidth - 1)) - CellSize * 2;
		float Z = 0;
		float rotZ = 0;

		if (OutCorners.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutCorners.Num() - 1);
			UStaticMesh* RandomMesh = OutCorners[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutCorners is empty"));
		}
	}

	// Corner 3
	{
		float X = (0 - CellSize * (OutDepth - 1)) - CellSize * 2;
		float Y = (0 - CellSize * (OutWidth - 1)) - CellSize * 2;
		float Z = 0;
		float rotZ = 270;

		if (OutCorners.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutCorners.Num() - 1);
			UStaticMesh* RandomMesh = OutCorners[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutCorners is empty"));
		}
	}

	// Corner 4
	{
		float X = (0 - CellSize * (OutDepth - 1)) - CellSize * 2;
		float Y = CellSize * OutWidth + CellSize;
		float Z = 0;
		float rotZ = 180;

		if (OutCorners.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, OutCorners.Num() - 1);
			UStaticMesh* RandomMesh = OutCorners[RandomIndex];
			SpawnMesh(RandomMesh, X, Y, Z, rotZ, true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutCorners is empty"));
		}
	}
}

void ACastleGenerator::GenerateCenter()
{
	int i = 0;
	for (FLine Line : DungeonColumns)
	{
		int j = 0;
		for (int Height : Line.Height)
		{
			for (int h = 0; h < Height; h++)
			{
				float X = i * (2 * CellSize) - CellSize * (DungeonColumns.Num() - 1);
				float Y = j * (2 * CellSize) - CellSize * (Line.Height.Num() - 1);
				float Z = (2 * CellSize) * h;
				float rot = FMath::RandRange(0, 4) * 90;
				if (h == 0) // Base
				{
					int32 RandomIndex = FMath::RandRange(0, DungeonsFloors.Num() - 1);
					UStaticMesh* RandomMesh = DungeonsFloors[RandomIndex];
					SpawnMesh(RandomMesh, X, Y, Z, rot);
				}
				else if (h < Height - 1) // Not a Roof
				{
					int32 RandomIndex = FMath::RandRange(0, DungeonsWalls.Num() - 1);
					UStaticMesh* RandomMesh = DungeonsWalls[RandomIndex];
					SpawnMesh(RandomMesh, X, Y, Z, rot);
				}
				else // Roof
				{
					int32 RandomIndex = FMath::RandRange(0, DungeonsRoofs.Num() - 1);
					UStaticMesh* RandomMesh = DungeonsRoofs[RandomIndex];
					SpawnMesh(RandomMesh, X, Y, Z, rot);
				}
			}
			j++;
		}
		i++;
	}
}

void ACastleGenerator::GeneratePerlinNoiseDungeon()
{
	int InDepth = OutDepth / 2;
	int InWidth = OutWidth / 2;

	for (int32 Y = 0; Y < InDepth; ++Y)
	{
		FLine NewLine = FLine();
		DungeonColumns.Add(NewLine);

		for (int32 X = 0; X < InWidth; ++X)
		{
			float NoiseValue = FMath::PerlinNoise2D(FVector2D(X, Y) * Scale);
			int32 MappedValue = FMath::Clamp(FMath::RoundToInt((NoiseValue + 1.0f) * 127.5f), 0, 255);
			
			int HeightValue;

			if (MappedValue < 20)
			{
				HeightValue = 1;
			}
			else if (MappedValue < 50)
			{
				HeightValue = 2;
			}
			else if (MappedValue < 100)
			{
				HeightValue = 3;
			}
			else if (MappedValue < 150)
			{
				HeightValue = 4;
			}
			else if (MappedValue < 200)
			{
				HeightValue = 5;
			}
			else
			{
				HeightValue = 6;
			}

			DungeonColumns[Y].Height.Add(HeightValue);
		}
	}
}

void ACastleGenerator::GenerateSize()
{
	OutDepth = FMath::RandRange(10, 20);
	OutWidth = FMath::RandRange(10, 20);
}

void ACastleGenerator::SpawnMesh(UStaticMesh* mesh, float X, float Y, float Z, float rotZ, bool ApplyNormal)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	AStaticMeshActor* Mesh = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnInfo);
	Mesh->GetRootComponent()->Mobility = EComponentMobility::Movable;

	const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	Mesh->GetRootComponent()->AttachToComponent(GetRootComponent(), AttachmentRules);

	Location = FVector(X, Y, Z);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(X, Y, Z) + FVector(0, 0, 100000), GetActorLocation() + FVector(X, Y, Z) + FVector(0, 0, -100000), ECollisionChannel::ECC_Visibility);
	Mesh->SetActorLocation(Hit.ImpactPoint);

	Rotation = FRotator(0.0f, rotZ, 0.0f);

	Mesh->SetActorRelativeRotation(Rotation);

	Mesh->GetStaticMeshComponent()->SetStaticMesh(mesh);

	Mesh->SetActorScale3D(FVector(5, 5, 5));

	
}