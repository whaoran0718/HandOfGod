// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHexitile> TileBlueprint;

	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, AHexitile*> Tiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* SeaLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Subdivision = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseVariantion = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseScale = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
