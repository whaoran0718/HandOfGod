// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Hexitile.generated.h"

UCLASS()
class HANDOFGOD_API AHexitile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexitile();

	void CreateMesh(const TArray<FVector>& vectors);

	void AddNeighbor(AHexitile* neighbor);

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
