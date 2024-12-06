// Fill out your copyright notice in the Description page of Project Settings.


#include "Attractor.h"

// Sets default values
AAttractor::AAttractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

// Called when the game starts or when spawned
void AAttractor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

