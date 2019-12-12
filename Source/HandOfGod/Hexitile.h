// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

//terrain type
UENUM(BlueprintType)
enum class ETerrain : uint8
{
    MOUNTAIN = 0,
    PLANE = 1,
    OCEAN = 2,
	RUINED = 3
};

class HANDOFGOD_API Hexitile
{	
public:
	Hexitile() {}
	Hexitile(const FVector& _center) 
		: center(_center), direction(_center.GetSafeNormal()), terrainType(ETerrain::PLANE)
	{}

	void CreateMesh(
		const TArray<FVector>& vertices,
		TArray<FVector>& outVertices,
		TArray<int>& outTriangles,
		TArray<FVector>& outNormals,
		float scale = 1.f,
		int offset = 0
	);

	// vertex buffer with only tile plane
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;

	// tile parameters
	ETerrain terrainType;
	FVector center;
	FVector direction;
	float height;
	TArray<Hexitile*> Neighbors;
};
