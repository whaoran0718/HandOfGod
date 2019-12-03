// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "Hexasphere.h"
#include "SimplexNoiseBPLibrary.h"

// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SeaLevel = CreateDefaultSubobject<UProceduralMeshComponent>("SeaLevel");
	SetRootComponent(SeaLevel);
	SeaLevel->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	Hexasphere* sphere = new Hexasphere();
	sphere->Generate(Radius, Subdivision);

	TArray<FVector> sea_v;
	TArray<int> sea_t;
	TArray<FVector> sea_n;
	for (auto& tile : sphere->tiles)
	{
		FActorSpawnParameters parameters;
		parameters.Owner = this;
		AHexitile* hexitile = GetWorld()->SpawnActor<AHexitile>(TileBlueprint, parameters);
		hexitile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		hexitile->CreateMesh(tile.Value->boundary);
		Tiles.Add(tile.Key->pos, hexitile);

		int stackCount = sea_v.Num();
		sea_v.Append(hexitile->Vertices);
		sea_n.Append(hexitile->Normals);
		for (int i = 0; i < hexitile->Triangles.Num(); i++)
			sea_t.Add(stackCount + hexitile->Triangles[i]);

		FVector posUniform = tile.Key->pos.GetSafeNormal();
		float noise = USimplexNoiseBPLibrary::SimplexNoise3D(posUniform.X, posUniform.Y, posUniform.Z, NoiseScale);
		float rate = (noise * 2 - 1) * NoiseVariantion / Radius + 1;
		hexitile->SetActorRelativeScale3D(FVector(rate));
	}

	SeaLevel->CreateMeshSection_LinearColor(0, sea_v, sea_t, sea_n, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	SeaLevel->SetMaterial(0, SeaMaterial);

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

