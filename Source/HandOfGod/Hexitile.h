// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Hexitile.generated.h"

#define MAXPOPULATION 100

//terrain type
UENUM(BlueprintType)
enum class ETerrain : uint8
{
    MOUNTAIN = 0,
    PLANE = 1,
    OCEAN = 2
};

UCLASS()
class HANDOFGOD_API AHexitile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexitile();

	void CreateMesh(const TArray<FVector>& vertices);

	void AddNeighbor(AHexitile* neighbor);

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
    void Migration();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float temperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int population;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETerrain terrainType = ETerrain::PLANE;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	UProceduralMeshComponent* TileMesh;

	UPROPERTY(BlueprintReadOnly)
	TArray<AHexitile*> Neighbors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
