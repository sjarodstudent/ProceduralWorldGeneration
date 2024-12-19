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
	GenerateOutWalls();
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
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
			SpawnMesh(RandomMesh, X, Y, Z, rotZ);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OutCorners is empty"));
		}
	}
}

void ACastleGenerator::SpawnMesh(UStaticMesh* mesh, float X, float Y, float Z, float rotZ)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	AStaticMeshActor* Mesh = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnInfo);

	Mesh->GetRootComponent()->Mobility = EComponentMobility::Movable;

	const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	Mesh->GetRootComponent()->AttachToComponent(GetRootComponent(), AttachmentRules);

	Location = FVector(X, Y, Z);
	Mesh->SetActorRelativeLocation(Location);

	Rotation = FRotator(0.0f, rotZ, 0.0f);
	Mesh->SetActorRelativeRotation(Rotation);

	Mesh->GetStaticMeshComponent()->SetStaticMesh(mesh);
}

