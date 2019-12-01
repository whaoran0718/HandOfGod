// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, AHexitile*> Tiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Planet)
	float Radius = 400;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= Planet)
	int32 Subdivision = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
