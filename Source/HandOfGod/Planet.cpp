// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "Hexasphere.h"
#include "SimplexNoiseBPLibrary.h"

// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HexaMesh = CreateDefaultSubobject<UProceduralMeshComponent>("PlanetMesh");
	SetRootComponent(HexaMesh);
	HexaMesh->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	Spawn();
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < WarningColors.Num(); i++)
		WarningColors[i] = FLinearColor::Black;

	for (auto& tile : LaunchPlane)
	{
		for (int i = 0; i < tile.Key->Vertices.Num(); i++)
		{
			WarningColors[i + PlaneTileBufferOffset[tile.Key].Value] = FLinearColor::White * tile.Value;
		}
	}
	HexaMesh->UpdateMeshSection_LinearColor(1, PlaneVertices, PlaneNormals, TArray<FVector2D>(), PlaneColors, TArray<FProcMeshTangent>());
	HexaMesh->UpdateMeshSection_LinearColor(3, WarningVertices, WarningNormals, TArray<FVector2D>(), WarningColors, TArray<FProcMeshTangent>());
	LaunchPlane.Reset();
}

int APlanet::ShowLaunchPlane(FVector position, float meterRadius, bool ruin)
{
	FVector relativeDir;
	float cosRange = FMath::Sqrt(1 - FMath::Pow(meterRadius / Radius, 2));
	FQuat invRot = GetActorTransform().GetRotation().Inverse();
	relativeDir = invRot.RotateVector(position.GetSafeNormal());

	int planeCount = 0;
	for (auto& tile : Tiles)
	{
		float distance = 1 - (position.Size() - Radius) / WarningMaxDistance;
		if (distance > 0 && FVector::DotProduct(tile.Key, relativeDir) > cosRange && tile.Value->terrainType == ETerrain::PLANE)
		{
			if (LaunchPlane.Contains(tile.Value))
				LaunchPlane[tile.Value] = FMath::Min(FMath::Max(distance, LaunchPlane[tile.Value]), 1.f);
			else
				LaunchPlane.Add(tile.Value, distance);
			planeCount++;

			if (ruin)
			{
				tile.Value->terrainType = ETerrain::RUINED;
				PlaneCount--;
				for (int i = 0; i < tile.Value->Vertices.Num() * 4; i++)
				{
					int idx = i + PlaneTileBufferOffset[tile.Value].Key;
					PlaneVertices[idx] *= (1 - NoiseVariantion / Radius);
					PlaneColors[idx] = FLinearColor::White;
				}
			}
		}
	}

	return planeCount;
}

int APlanet::GetPlaneCount()
{
	return Tiles.Num();
}

void APlanet::Respawn(bool regenerate)
{
	if (regenerate)
	{
		PlaneCount = 0;
		for (auto& tile : Tiles)
		{
			if (tile.Value->terrainType == ETerrain::RUINED)
			{
				for (int i = 0; i < tile.Value->Vertices.Num() * 4; i++)
				{
					int idx = i + PlaneTileBufferOffset[tile.Value].Key;
					PlaneVertices[idx] *= tile.Value->height + 1;
					PlaneColors[idx] = FLinearColor::Black;
				}
				tile.Value->terrainType = ETerrain::PLANE;
			}
			if (tile.Value->terrainType == ETerrain::PLANE)
				PlaneCount++;
		}
	}
	else
	{
		for (auto& tile : Tiles)
		{
			delete tile.Value;
		}
		Tiles.Reset();
		PlaneVertices.Reset();
		PlaneTriangles.Reset();
		PlaneNormals.Reset();
		PlaneColors.Reset();
		WarningVertices.Reset();
		WarningTriangles.Reset();
		WarningNormals.Reset();
		WarningColors.Reset();

		PlaneTileBufferOffset.Reset();
		LaunchPlane.Reset();

		Spawn();
	}
}

void APlanet::Spawn()
{
	Hexasphere* sphere = new Hexasphere();
	sphere->Generate(Radius, Subdivision);
	PlaneCount = 0;

	TArray<FVector> sea_v, sea_n;
	TArray<int> sea_t;
	TArray<FVector> mount_v, mount_n;
	TArray<int> mount_t;
	TArray<FVector> ocean_v, ocean_n;
	TArray<int> ocean_t;
	int section = 0;
	for (auto& tile : sphere->tiles)
	{
		FActorSpawnParameters parameters;
		parameters.Owner = this;
		Hexitile* hexitile = new Hexitile(tile.Value->centroid->pos);
		Tiles.Add(tile.Key->dir, hexitile);

		FVector posUniform = hexitile->direction;
		float noise = USimplexNoiseBPLibrary::SimplexNoise3D(posUniform.X, posUniform.Y, posUniform.Z, NoiseScale);
		if (TerrainRemapCurve)
			noise = TerrainRemapCurve->GetFloatValue(noise);
		float rate = noise * NoiseVariantion / Radius + 1;

		hexitile->terrainType = noise > PlaneTileRange[1] ? ETerrain::MOUNTAIN : (noise < PlaneTileRange[0] ? ETerrain::OCEAN : ETerrain::PLANE);

		TArray<FVector> tmp_rock_v, tmp_rock_n;
		TArray<int> tmp_rock_t;

		switch (hexitile->terrainType)
		{
		case ETerrain::MOUNTAIN:
		{
			hexitile->CreateMesh(tile.Value->boundary, tmp_rock_v, tmp_rock_t, tmp_rock_n, rate, mount_v.Num());
			mount_v.Append(tmp_rock_v);
			mount_t.Append(tmp_rock_t);
			mount_n.Append(tmp_rock_n);
			break;
		}
		case ETerrain::PLANE:
		{
			PlaneTileBufferOffset.Add(hexitile, TPair<int, int>(PlaneVertices.Num(), WarningNormals.Num()));
			hexitile->CreateMesh(tile.Value->boundary, tmp_rock_v, tmp_rock_t, tmp_rock_n, rate, PlaneVertices.Num());
			PlaneVertices.Append(tmp_rock_v);
			PlaneTriangles.Append(tmp_rock_t);
			PlaneNormals.Append(tmp_rock_n);

			WarningNormals.Append(hexitile->Normals);
			for (int i = 0; i < hexitile->Triangles.Num(); i++)
				WarningTriangles.Add(WarningVertices.Num() + hexitile->Triangles[i]);
			WarningVertices.Append(hexitile->Vertices);

			PlaneCount++;

			break;
		}
		case ETerrain::OCEAN:
		{
			hexitile->CreateMesh(tile.Value->boundary, tmp_rock_v, tmp_rock_t, tmp_rock_n, rate, ocean_v.Num());
			ocean_v.Append(tmp_rock_v);
			ocean_t.Append(tmp_rock_t);
			ocean_n.Append(tmp_rock_n);

			break;
		}
		default: break;
		}
	}
	PlaneColors.Reset(PlaneVertices.Num());
	for (int i = 0; i < PlaneVertices.Num(); i++)
		PlaneColors.Push(FLinearColor::Black);

	WarningColors.Reset(WarningVertices.Num());
	for (int i = 0; i < WarningVertices.Num(); i++)
		WarningColors.Push(FLinearColor::Black);

	for (auto& tile : Tiles)
	{
		for (int i = 0; i < tile.Value->Triangles.Num(); i++)
			sea_t.Add(sea_v.Num() + tile.Value->Triangles[i]);
		for (int i = 0; i < tile.Value->Vertices.Num(); i++)
		{
			sea_v.Add(tile.Value->Vertices[i] / (tile.Value->height + 1));
			sea_n.Add(tile.Value->Vertices[i].GetSafeNormal());
		}
	}


	HexaMesh->CreateMeshSection_LinearColor(0, mount_v, mount_t, mount_n, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	HexaMesh->CreateMeshSection_LinearColor(1, PlaneVertices, PlaneTriangles, PlaneNormals, TArray<FVector2D>(), PlaneColors, TArray<FProcMeshTangent>(), false);
	HexaMesh->CreateMeshSection_LinearColor(2, ocean_v, ocean_t, ocean_n, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	HexaMesh->CreateMeshSection_LinearColor(3, WarningVertices, WarningTriangles, WarningNormals, TArray<FVector2D>(), WarningColors, TArray<FProcMeshTangent>(), false);
	HexaMesh->CreateMeshSection_LinearColor(4, sea_v, sea_t, sea_n, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	HexaMesh->SetMaterial(0, MountainTileMaterial);
	HexaMesh->SetMaterial(1, PlaneTileMaterial);
	HexaMesh->SetMaterial(2, OceanTileMaterial);
	HexaMesh->SetMaterial(3, HighlightMaterial);
	HexaMesh->SetMaterial(4, SeaMaterial);

	for (auto& tile : sphere->tiles)
	{
		for (auto* neighbor : tile.Value->neighbors)
			Tiles[tile.Key->dir]->Neighbors.Add(Tiles[neighbor->dir]);
	}

	delete sphere;
}

