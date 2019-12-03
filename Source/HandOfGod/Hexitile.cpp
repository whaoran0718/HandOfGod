// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexitile.h"

const float rate = 1000.f;
float time_acc = 0.f;

// Sets default values
AHexitile::AHexitile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TileMesh");
	SetRootComponent(TileMesh);
	TileMesh->bUseAsyncCooking = true;

    temperature = 20.0;
    population = 0;
}

void AHexitile::CreateMesh(const TArray<FVector>& vertices)
{
	TArray<FVector> verWithOrigin = vertices;
	TArray<int32> triangles;
	for (int i = 1; i < verWithOrigin.Num() - 1; i++)
	{
		triangles.Push(0);
		triangles.Push(i);
		triangles.Push(i + 1);
	}

	TArray<FVector> normals;
	FVector normal = FVector::ZeroVector;
	for (int i = 0; i < verWithOrigin.Num(); i++)
		normal += verWithOrigin[i];
	normal.Normalize();
	normals.Init(normal, verWithOrigin.Num());

	Vertices = vertices;
	Triangles = triangles;
	Normals = normals;

	for (int i = 0; i < verWithOrigin.Num(); i++)
	{
		int next = (i + 1) % verWithOrigin.Num();
		triangles.Push(i);
		triangles.Push(verWithOrigin.Num());
		triangles.Push(next);

		FVector n = FVector::CrossProduct(-verWithOrigin[i], verWithOrigin[next]);
		n.Normalize();
		normals.Push(n);
		normals.Push(n);
		normals.Push(n);
	}
	verWithOrigin.Push(FVector::ZeroVector);

	TileMesh->CreateMeshSection_LinearColor(0, verWithOrigin, triangles, normals, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
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
    
    time_acc += DeltaTime;

	Super::Tick(DeltaTime);

    //only updating population for plane terrain
    if (terrainType == ETerrain::PLANE)
    {
        //update population per 3s
        if (time_acc > 3.f) {
            time_acc = 0;
            population += 5;
        }
    }

    //migrate when population reaches the cap on this tile
    if (population > MAXPOPULATION) Migration();
}

void AHexitile::Migration()
{

}

