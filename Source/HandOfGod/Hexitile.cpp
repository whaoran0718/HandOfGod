// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexitile.h"

void Hexitile::CreateMesh(
	const TArray<FVector>& vertices,
	TArray<FVector>& outVertices,
	TArray<int>& outTriangles,
	TArray<FVector>& outNormals,
	float scale,
	int offset
)
{
	height = scale - 1;

	TArray<int32> triangles;
	for (int i = 1; i < vertices.Num() - 1; i++)
	{
		triangles.Push(0);
		triangles.Push(i);
		triangles.Push(i + 1);
	}

	TArray<FVector> normals;
	FVector normal = FVector::ZeroVector;
	for (int i = 0; i < vertices.Num(); i++)
	{
		Vertices.Add(vertices[i] * scale);
		normal += vertices[i];
	}
	normal.Normalize();
	normals.Init(normal, vertices.Num());

	Triangles = triangles;
	Normals = normals;

	TArray<FVector> VerRdt = Vertices;
	for (int i = 0; i < Vertices.Num(); i++)
	{
		int next = (i + 1) % Vertices.Num();
		VerRdt[i] = Vertices[i];
		VerRdt.Push(Vertices[i]);
		VerRdt.Push(FVector::ZeroVector);
		VerRdt.Push(Vertices[next]);

		triangles.Push(VerRdt.Num() - 3);
		triangles.Push(VerRdt.Num() - 2);
		triangles.Push(VerRdt.Num() - 1);

		FVector n = -FVector::CrossProduct(-Vertices[i], Vertices[next]);
		n.Normalize();
		normals.Push(n);
		normals.Push(n);
		normals.Push(n);
	}

	outVertices = VerRdt;
	outNormals = normals;
	for (int i = 0; i < triangles.Num(); i++)
		outTriangles.Add(triangles[i] + offset);
}

