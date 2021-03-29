#include "Generation/MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <DrawDebugHelpers.h>
#include "Generation/GenerationUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/CaveActor.h"

AMeshGenerator::AMeshGenerator()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;

	_Noise = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("Noise"));
}

void AMeshGenerator::AddPoint(FVector hitPoint, bool isAddition)
{
	for (FVector4& point : Points)
	{
		if(FVector::Dist(point * CubeSize, hitPoint) < AdditionRadius)
		{
			float value = point.W;
			value += isAddition ? -AdditionValue : AdditionValue;
			point.W = value;
		}
	}
	GenerateMesh();
}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	_Noise->SetupFastNoise(EFastNoise_NoiseType::PerlinFractal, 1337, 0.02);

	TArray<AActor*> caves;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACaveActor::StaticClass(), caves);

	for (int x = 0; x < Bounds.X; ++x)
	{
		for (int y = 0; y < Bounds.Y; ++y)
		{
			for (int z = 0; z < Bounds.Z; ++z)
			{
				FVector4 point(x, y, z, 0);
				FVector scaledPoint = point * CubeSize;

				float height = UKismetMathLibrary::NormalizeToRange(point.Z, 0.0f, Bounds.Z);
				float noise = FMath::PerlinNoise3D(FVector(x,y,z) * NoiseScale);
				noise = (noise + 1) / 2;
				point.W = noise + height / 2;


				if(x == 0 || y == 0 || x >= Bounds.X - 2 || y >= Bounds.Y - 2 || z >= Bounds.Z - 2)
				{
					point.W = FMath::RandRange(0.15f, 0.25f);
				}

				if(point.Z == 0 && point.W > IsoLevel)
				{
					point.W = 0.49f;
					//point.W = 0.5f - FMath::RandRange(0.15, 0.35f);
				}

				//FVector noisePoint = point * NoiseScale;
				//float noise = _Noise->GetNoise3D(noisePoint.X, noisePoint.Y, noisePoint.Z);
				//UE_LOG(LogTemp, Log, TEXT("noise: %f"), noise);
				//noise = point.Z + noise * Amplitude;
				//noise = FMath::Clamp(noise, 0.0f, static_cast<float>(MaxNoiseValue));
				//noise = UKismetMathLibrary::NormalizeToRange(noise, 0.0f, MaxNoiseValue);
				//point.W = noise;
				//UE_LOG(LogTemp, Log, TEXT("noise: %s"), *point.ToString());

				//if (UKismetMathLibrary::IsPointInBox(scaledPoint, BoxOrigin, BoxExtent))
				//{
				//	point.W = FMath::RandRange(0.15f, 0.5f);
				//}
				//
				//for (AActor* actor : caves)
				//{
				//	ACaveActor* cave = Cast<ACaveActor>(actor);
				//	if(cave)
				//	{
				//		if(cave->IsPointInside(scaledPoint))
				//		{
				//			point.W = 1;
				//		}
				//	}
				//}

				//if(UKismetMathLibrary::IsPointInBox(scaledPoint, BoxOrigin, BoxExtent))


				//float dist = FVector::DistSquared(scaledPoint, SphereCenter);
				//if (dist < SphereRadius * SphereRadius * 3)
				//{
				//	point.W = 0;
				//}
				//else
				//{
				//	point.W = 1;
				//}

				if (DrawDebugPoints /*&& point.W < IsoLevel*/)
				{
					float value = point.W * 255;
					FColor color = FColor(value, value, value);
					//FColor color = point.W ? FColor::Green : FColor::Red;
					DrawDebugSphere(GetWorld(), scaledPoint, 10, 4, color, false, 25);
				}
				Points.Emplace(point);
			}
		}
	}

	if(DoGenerateMesh)
	{
		GenerateMesh();
	}
}

void AMeshGenerator::GenerateMesh()
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

	TArray<Triangle> triangles;

	for (int x = 0; x < Bounds.X; ++x)
	{
		for (int y = 0; y < Bounds.Y; ++y)
		{
			for (int z = 0; z < Bounds.Z; ++z)
			{
				if (x >= Bounds.X - 1 || y >= Bounds.Y - 1 || z >= Bounds.Z - 1) {
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

				float isoLevel = IsoLevel;
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

					int a1 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i+1]][0];
					int b1 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i+1]][1];

					int a2 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i+2]][0];
					int b2 = GenerationUtils::Edges[GenerationUtils::TriTable[cubeIndex][i+2]][1];
				
					Triangle tri;
					tri.vertexA = InterpolateVertex(cubeCorners[a0], cubeCorners[b0], isoLevel) * CubeSize;
					tri.vertexB = InterpolateVertex(cubeCorners[a1], cubeCorners[b1], isoLevel) * CubeSize;
					tri.vertexC = InterpolateVertex(cubeCorners[a2], cubeCorners[b2], isoLevel) * CubeSize;
					triangles.Emplace(tri);
				}
			}
		}
	}

	TArray<FVector> vertices;
	vertices.Init(FVector::ZeroVector, triangles.Num() * 3);
	TArray<int> indices;
	indices.Init(-1, triangles.Num() * 3);

	for(int i = 0; i < triangles.Num(); ++i)
	{
		for(int j = 0; j < 3; ++j)
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

FVector AMeshGenerator::InterpolateVertex(FVector4 a, FVector4 b, float isoLevel)
{
	//return a + (b - a) / 2;

	float t = (isoLevel - a.W) / (b.W - a.W);
	return a + t * (b - a);
}

int AMeshGenerator::IndexFromCoord(int x, int y, int z)
{
	return x + y * Bounds.Y + z * Bounds.X * Bounds.Y;
}
