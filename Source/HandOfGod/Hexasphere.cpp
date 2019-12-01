// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexasphere.h"

Hexasphere::Hexasphere()
{}

Hexasphere::~Hexasphere()
{
	for (auto& tile : tiles)
		delete tile.Value;

	for (auto& point : points)
		delete point.Value;

	for (auto* face : faces)
		delete face;
}

void Hexasphere::Generate(float sphereRadius, int numDivisions)
{
	radius = FMath::Max(100.f, sphereRadius);
	numDivisions = FMath::Max(1, numDivisions);
	float tao = 1.61803399;
	TArray<HxPoint*> corners;
	corners.Push(new HxPoint(FVector(1, tao, 0)));
	corners.Push(new HxPoint(FVector(-1, tao, 0)));
	corners.Push(new HxPoint(FVector(1, -tao, 0)));
	corners.Push(new HxPoint(FVector(-1, -tao, 0)));
	corners.Push(new HxPoint(FVector(0, 1, tao)));
	corners.Push(new HxPoint(FVector(0, -1, tao)));
	corners.Push(new HxPoint(FVector(0, 1, -tao)));
	corners.Push(new HxPoint(FVector(0, -1, -tao)));
	corners.Push(new HxPoint(FVector(tao, 0, 1)));
	corners.Push(new HxPoint(FVector(-tao, 0, 1)));
	corners.Push(new HxPoint(FVector(tao, 0, -1)));
	corners.Push(new HxPoint(FVector(-tao, 0, -1)));

	points.Empty();
	for (int i = 0; i < corners.Num(); i++)
		points.Add(corners[i]->pos, corners[i]);

	TArray<HxFace*> initFaces;
	initFaces.Push(new HxFace(corners[0], corners[1], corners[4], false));
	initFaces.Push(new HxFace(corners[1], corners[9], corners[4], false));
	initFaces.Push(new HxFace(corners[4], corners[9], corners[5], false));
	initFaces.Push(new HxFace(corners[5], corners[9], corners[3], false));
	initFaces.Push(new HxFace(corners[2], corners[3], corners[7], false));
	initFaces.Push(new HxFace(corners[3], corners[2], corners[5], false));
	initFaces.Push(new HxFace(corners[7], corners[10], corners[2], false));
	initFaces.Push(new HxFace(corners[0], corners[8], corners[10], false));
	initFaces.Push(new HxFace(corners[0], corners[4], corners[8], false));
	initFaces.Push(new HxFace(corners[8], corners[2], corners[10], false));
	initFaces.Push(new HxFace(corners[8], corners[4], corners[5], false));
	initFaces.Push(new HxFace(corners[8], corners[5], corners[2], false));
	initFaces.Push(new HxFace(corners[1], corners[0], corners[6], false));
	initFaces.Push(new HxFace(corners[11], corners[1], corners[6], false));
	initFaces.Push(new HxFace(corners[3], corners[9], corners[11], false));
	initFaces.Push(new HxFace(corners[6], corners[10], corners[7], false));
	initFaces.Push(new HxFace(corners[3], corners[11], corners[7], false));
	initFaces.Push(new HxFace(corners[11], corners[6], corners[7], false));
	initFaces.Push(new HxFace(corners[6], corners[0], corners[10], false));
	initFaces.Push(new HxFace(corners[9], corners[1], corners[11], false));

	faces.Empty();
	for (int f = 0; f < initFaces.Num(); f++)
	{
		TArray<HxPoint*> previous;
		TArray<HxPoint*> origin, left, right;
		origin.Push(initFaces[f]->points[0]);
		left = initFaces[f]->points[0]->GetSubdivide(initFaces[f]->points[1], numDivisions, this, true);
		right = initFaces[f]->points[0]->GetSubdivide(initFaces[f]->points[2], numDivisions, this, true);
		for (int i = 1; i <= numDivisions; i++)
		{
			previous = origin;
			origin = left[i]->GetSubdivide(right[i], i, this, i == numDivisions);
			for (int j = 0; j < i; j++)
			{
				faces.Push(new HxFace(previous[j], origin[j], origin[j + 1]));
				if (j > 0)
					faces.Push(new HxFace(previous[j - 1], previous[j], origin[j]));
			}
		}
	}

	for (auto& point : points)
		point.Value->pos = point.Value->GetPointOnRadius(radius)->pos;
	
	for (auto& point : points)
		tiles.Add(point.Value, new HxTile(point.Value));

	for (auto* face : initFaces)
		delete face;
}

HxPoint::HxPoint(const FVector& point) : pos(point)
{}

TArray<HxPoint*> HxPoint::GetSubdivide(HxPoint* point, int count, Hexasphere* sphere, bool checkRepeat)
{
	if (checkRepeat && sphere->pointsOnEdge.Contains(TPair<HxPoint*, HxPoint*>(this, point)))
		return sphere->pointsOnEdge[TPair<HxPoint*, HxPoint*>(this, point)];

	TArray<HxPoint*> result;
	result.Push(this);
	for (int i = 1; i < count; i++)
	{
		HxPoint* newpoint = GetPointByPercent(point, i * 1.0 / count);
		sphere->points.Add(newpoint->pos, newpoint);
		result.Push(newpoint);
	}
	result.Push(point);

	if (checkRepeat)
	{
		sphere->pointsOnEdge.Add(TPair<HxPoint*, HxPoint*>(this, point), result);
		TArray<HxPoint*> resultRev;
		for (int i = 0; i < result.Num(); i++)
			resultRev.Push(result[result.Num() - i - 1]);
		sphere->pointsOnEdge.Add(TPair<HxPoint*, HxPoint*>(point, this), resultRev);
	}

	return result;
}

HxPoint* HxPoint::GetPointByPercent(HxPoint* point, float percent)
{
	percent = FMath::Clamp(percent, 0.f, 1.f);
	return new HxPoint(pos * (1 - percent) + point->pos * percent);
}

HxPoint* HxPoint::GetMidpoint(HxPoint* point)
{
	return GetPointByPercent(point, 0.5);
}

HxPoint* HxPoint::GetPointOnRadius(float radius)
{
	return new HxPoint(pos * radius / pos.Size());
}

void HxPoint::AttachFace(HxFace* face)
{
	return faces.Push(face);
}

TArray<HxFace*> HxPoint::GetOrderedFaces()
{
	TArray<HxFace*> temp = faces;
	TArray<HxFace*> result;

	for (int i = 0; i < faces.Num(); i++)
	{
		if (i == 0)
		{
			result.Push(temp[0]);
			temp.RemoveAt(0);
		}
		else
		{
			for (int j = 0; j < temp.Num(); j++)
			{
				if (temp[j]->isAdjacentTo(result[i - 1]))
				{
					result.Push(temp[j]);
					temp.RemoveAt(j);
					break;
				}
			}
		}
	}
	return result;
}

HxFace::HxFace(HxPoint* p0, HxPoint* p1, HxPoint* p2, bool attachToPoint)
{
	points.Push(p0);
	points.Push(p1);
	points.Push(p2);

	if (attachToPoint)
	{
		p0->AttachFace(this);
		p1->AttachFace(this);
		p2->AttachFace(this);
	}
}

TArray<HxPoint*> HxFace::GetPointsExclude(HxPoint* p0, HxPoint* p1)
{
	TArray<HxPoint*> result;
	for (int i = 0; i < points.Num(); i++)
	{
		if (points[i] != p0 && points[i] != p1)
			result.Push(points[i]);
	}
	return result;
}

bool HxFace::isAdjacentTo(HxFace* face)
{
	int count = 0;
	for (int i = 0; i < points.Num(); i++)
	{
		for (int j = 0; j < face->points.Num(); j++)
		{
			if (points[i] == face->points[j])
				count++;
		}
	}
	return count == 2;
}

FVector HxFace::GetCentroid()
{
	FVector centroid = FVector::ZeroVector;
	for (int i = 0; i < points.Num(); i++)
	{
		centroid += points[i]->pos;
	}
	return centroid / points.Num();
}

HxTile::HxTile(HxPoint* center)
{
	centroid = center;
	TArray<HxFace*> subfaces = centroid->GetOrderedFaces();
	for (int f = 0; f < subfaces.Num(); f++)
	{
		boundary.Push(subfaces[f]->GetCentroid());
		TArray<HxPoint*> otherPoints = subfaces[f]->GetPointsExclude(centroid);
		for (auto* point : otherPoints)
			neighbors.AddUnique(point);
	}

	FVector normal = FVector::CrossProduct(boundary[1] - boundary[0], boundary[2] - boundary[1]);
	if (FVector::DotProduct(centroid->pos, normal) > 0)
	{
		for (int i = 0; i < (int)(boundary.Num() / 2); i++)
			boundary.Swap(i, boundary.Num() - i - 1);
	}
}
