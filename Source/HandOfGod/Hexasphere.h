// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class Hexasphere;
class HxPoint;
class HxFace;
class HxTile;

/**
 * 
 */
class HANDOFGOD_API Hexasphere
{
public:
	Hexasphere();
	~Hexasphere();

	void Generate(float sphereRadius, int numDivisions);

	float radius;
	TMap<FVector, HxPoint*> points;
	TMap<TPair<HxPoint*, HxPoint*>, TArray<HxPoint*>> pointsOnEdge;
	TArray<HxFace*> faces;
	TMap<HxPoint*, HxTile*> tiles;
};

class HANDOFGOD_API HxPoint
{
public:
	HxPoint(const FVector& point);
	TArray<HxPoint*> GetSubdivide(HxPoint* point, int count, Hexasphere* sphere, bool checkRepeat = false);
	HxPoint* GetPointByPercent(HxPoint* point, float percent);
	HxPoint* GetMidpoint(HxPoint* point);
	HxPoint* GetPointOnRadius(float radius);
	void AttachFace(HxFace* face);
	TArray<HxFace*> GetOrderedFaces();

	FVector pos;
	FVector dir;
	TArray<HxFace*> faces;
};

class HANDOFGOD_API HxFace
{
public:
	HxFace(HxPoint* p0, HxPoint* p1, HxPoint* p2, bool attachToPoint = true);
	TArray<HxPoint*> GetPointsExclude(HxPoint* p0, HxPoint* p1 = NULL);
	bool isAdjacentTo(HxFace* face);
	FVector GetCentroid();

	TArray<HxPoint*> points;
};

class HANDOFGOD_API HxTile
{
public:
	HxTile(HxPoint* center);

	HxPoint* centroid;
	TArray<FVector> boundary;
	TArray<HxPoint*> neighbors;  
};
