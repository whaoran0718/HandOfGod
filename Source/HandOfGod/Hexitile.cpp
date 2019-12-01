// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexitile.h"

// Sets default values
AHexitile::AHexitile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TileMesh");
	SetRootComponent(TileMesh);
	TileMesh->bUseAsyncCooking = true;
}

void AHexitile::CreateMesh(const TArray<FVector>& vectors, FVector normal)
{
	TArray<int32> triangles;
	for (int i = 1; i < vectors.Num() - 1; i++)
	{
		triangles.Push(0);
		triangles.Push(i);
		triangles.Push(i + 1);
	}
	TArray<FVector> normals;
	if (normal != FVector::ZeroVector)
	{
		normals.Init(normal, vectors.Num());
	}
	else
	{
		for (int i = 0; i < vectors.Num(); i++)
			normal += vectors[i];
		normal.Normalize();
		normals.Init(normal, vectors.Num());
	}

	TileMesh->CreateMeshSection_LinearColor(0, vectors, triangles, normals, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void AHexitile::AddNeighbor(AHexitile * neighbor)
{
	Neighbors.Push(neighbor);
}

// Called when the game starts or when spawned
void AHexitile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexitile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

