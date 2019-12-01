// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "Hexasphere.h"

// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	Hexasphere* sphere = new Hexasphere();
	sphere->Generate(Radius, Subdivision);

	for (auto& tile : sphere->tiles)
	{
		AHexitile* hexitile = GetWorld()->SpawnActor<AHexitile>();
		hexitile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		hexitile->CreateMesh(tile.Value->boundary);
		Tiles.Add(tile.Key->pos, hexitile);
	}

	for (auto& tile : sphere->tiles)
	{
		for (auto* neighbor : tile.Value->neighbors)
			Tiles[tile.Key->pos]->AddNeighbor(Tiles[neighbor->pos]);
	}

	delete sphere;
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

