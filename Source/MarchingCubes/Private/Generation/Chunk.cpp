#include "Generation/Chunk.h"
#include "Kismet/KismetMathLibrary.h"
#include "Generation/GenerationUtils.h"
#include "ProceduralMeshComponent.h"

AChunk::AChunk()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;
}

void AChunk::Create(FVector origin, int numOfPoints, float noiseScale, float isoLevel, float cubeSize)
{
	struct Triangle
	{
		FVector vertexA;
		FVector vertexB;
		FVector vertexC;

		FVector operator [] (int i) const
		{
			switch (i) {
			case 0:
				return vertexA;
			case 1:
				return vertexB;
			default:
				return vertexC;
			}
		};
	};

	NumOfPoints = numOfPoints;
	for (int x = 0; x < NumOfPoints; ++x)
	{
		for (int y = 0; y < NumOfPoints; ++y)
		{
			for (int z = 0; z < NumOfPoints; ++z)
			{
				FVector4 point(x, y, z, 0);
				FVector realPoint = point + origin * (numOfPoints - 1);
				//float height = UKismetMathLibrary::NormalizeToRange(point.Z, 0.0f, Bounds.Z);
				float height = 0.0f;
				float noise = FMath::PerlinNoise3D(realPoint * noiseScale);
				noise = (noise + 1) / 2;
				point.W = noise + height / 2;

				//if (x == 0 || y == 0 || x >= Bounds.X - 2 || y >= Bounds.Y - 2 || z >= Bounds.Z - 2)
				//{
				//	point.W = FMath::RandRange(0.15f, 0.25f);
				//}
				//
				//if (point.Z < 3 && point.W > IsoLevel)
				//{
				//	point.W = 0.49f;
				//}
				Points.Emplace(point);
			}
		}
	}

	TArray<Triangle> triangles;
	for (int x = 0; x < NumOfPoints; ++x)
	{
		for (int y = 0; y < NumOfPoints; ++y)
		{
			for (int z = 0; z < NumOfPoints; ++z)
			{
				if (x >= NumOfPoints - 1 || y >= NumOfPoints - 1 || z >= NumOfPoints - 1) {
					break;
				}

				FVector4 cubeCorners[8] = {
					Points[IndexFromCoord(x, y, z)],
					Points[IndexFromCoord(x + 1, y, z)],
					Points[IndexFromCoord(x + 1, y, z + 1)],
					Points[IndexFromCoord(x, y, z + 1)],
					Points[IndexFromCoord(x, y + 1, z)],
					Points[IndexFromCoord(x + 1, y + 1, z)],
					Points[IndexFromCoord(x + 1, y + 1, z + 1)],
					Points[IndexFromCoord(x, y + 1, z + 1)]
				};

				int cubeIndex = 0;
				if (cubeCorners[0].W < isoLevel) cubeIndex |= 1;
				if (cubeCorners[1].W < isoLevel) cubeIndex |= 2;
				if (cubeCorners[2].W < isoLevel) cubeIndex |= 4;
				if (cubeCorners[3].W < isoLevel) cubeIndex |= 8;
				if (cubeCorners[4].W < isoLevel) cubeIndex |= 16;
				if (cubeCorners[5].W < isoLevel) cubeIndex |= 32;
				if (cubeCorners[6].W < isoLevel) cubeIndex |= 64;
				if (cubeCorners[7].W < isoLevel) cubeIndex |= 128;

				for (int i = 0; GenerationUtils::TriTable[cubeIndex][i] != -1; i += 3)
				{
					int a0 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i]][0];
					int b0 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i]][1];

					int a1 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i + 1]][0];
					int b1 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i + 1]][1];

					int a2 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i + 2]][0];
					int b2 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i + 2]][1];

					Triangle tri;
					tri.vertexA = (InterpolateVertex(cubeCorners[a0], cubeCorners[b0], isoLevel)) * cubeSize;
					tri.vertexB = (InterpolateVertex(cubeCorners[a1], cubeCorners[b1], isoLevel)) * cubeSize;
					tri.vertexC = (InterpolateVertex(cubeCorners[a2], cubeCorners[b2], isoLevel)) * cubeSize;
					triangles.Emplace(tri);
				}
			}
		}
	}

	TArray<FVector> vertices;
	vertices.Init(FVector::ZeroVector, triangles.Num() * 3);
	TArray<int> indices;
	indices.Init(-1, triangles.Num() * 3);

	for (int i = 0; i < triangles.Num(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			indices[i * 3 + j] = i * 3 + j;
			vertices[i * 3 + j] = triangles[i][j];
		}
	}

	TArray<FVector> normals;
	normals.Init(FVector::ZeroVector, vertices.Num());
	for (int i = 0; i < indices.Num(); i += 3)
	{
		FVector v1 = vertices[indices[i + 1]] - vertices[indices[i]];
		FVector v2 = vertices[indices[i + 2]] - vertices[indices[i]];
		FVector faceNormal = FVector::CrossProduct(v2, v1);
		faceNormal.Normalize();

		normals[indices[i]] += faceNormal;
		normals[indices[i + 1]] += faceNormal;
		normals[indices[i + 2]] += faceNormal;
	}

	for (int i = 0; i < normals.Num(); ++i)
	{
		normals[i].Normalize();
	}

	Mesh->CreateMeshSection_LinearColor(0, vertices, indices, normals, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

FVector AChunk::InterpolateVertex(FVector4 a, FVector4 b, float isoLevel)
{
	float t = (isoLevel - a.W) / (b.W - a.W);
	return a + t * (b - a);
}

int AChunk::IndexFromCoord(int x, int y, int z) const
{
	return x + y * NumOfPoints + z * NumOfPoints * NumOfPoints;
}