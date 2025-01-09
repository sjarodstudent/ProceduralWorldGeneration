// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CastleGenerator.generated.h"

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Height;
};

UCLASS()
class PWG_API ACastleGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	float Scale = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generator")
	int OutDepth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	int OutWidth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	float CellSize = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<UStaticMesh*> OutWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<UStaticMesh*> OutCorners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<UStaticMesh*> DungeonsWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<UStaticMesh*> DungeonsRoofs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<UStaticMesh*> DungeonsFloors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator")
	TArray<FLine> DungeonColumns;

public:	
	// Sets default values for this actor's properties
	ACastleGenerator();

	void GenerateOutWalls();
	void GenerateCenter();
	void GeneratePerlinNoiseDungeon();
	void GenerateSize();
	void SpawnMesh(UStaticMesh* mesh, float X, float Y, float Z, float rotZ, bool ApplyNormal = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
