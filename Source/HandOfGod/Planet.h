// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Materials/Material.h"
#include "Curves/CurveFloat.h"
#include "Hexitile.h"
#include "Planet.generated.h"

UCLASS()
class HANDOFGOD_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TMap<FVector, Hexitile*> Tiles;

	TArray<FVector> PlaneVertices;
	TArray<int> PlaneTriangles;
	TArray<FVector> PlaneNormals;
	TArray<FLinearColor> PlaneColors;
	TArray<FVector> WarningVertices;
	TArray<int> WarningTriangles;
	TArray<FVector> WarningNormals;
	TArray<FLinearColor> WarningColors;

	TMap<Hexitile*, TPair<int, int>> PlaneTileBufferOffset;
	TMap<Hexitile*, float> LaunchPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int PlaneCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* HexaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* MountainTileMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* PlaneTileMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* OceanTileMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* HighlightMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* SeaMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Subdivision = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseVariantion = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseScale = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TerrainRemapCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PlaneTileRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarningMaxDistance = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int ShowLaunchPlane(FVector position, float meterRadius, bool ruin = false);

	UFUNCTION(BlueprintCallable)
	int GetPlaneCount();

	UFUNCTION(BlueprintCallable)
	void Respawn(bool regenerate = false);

	void Spawn();
};
