#include "Generation/MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <DrawDebugHelpers.h>
#include "Generation/GenerationUtils.h"

AMeshGenerator::AMeshGenerator()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;

}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateMesh();
}

void AMeshGenerator::GenerateMesh()
{
	struct Triangle
	{
		FVector vertexA;
		FVector vertexB;
		FVector vertexC;
	};


	const int numOfPoints = 2;
	const float cubeSize = 100.0f;

	TArray<Triangle> triangles;
	TArray<FVector> vertices;
	TArray<int> indices;

	for (int x = 0; x < numOfPoints; ++x)
	{
		for (int y = 0; y < numOfPoints; ++y)
		{
			for (int z = 0; z < numOfPoints; ++z)
			{
				DrawDebugSphere(GetWorld(), FVector(x*cubeSize, y*cubeSize, z*cubeSize), 2, 12, FColor::Green, false, 25);

				if (x >= numOfPoints - 1 || y >= numOfPoints - 1 || z >= numOfPoints - 1) {
					return;
				}

				FVector4 cubeCorners[8] = {
					FVector(x, y, z),
					FVector(x + 1, y, z),
					FVector(x + 1, y, z + 1),
					FVector(x, y, z + 1),
					FVector(x, y + 1, z),
					FVector(x + 1, y + 1, z),
					FVector(x + 1, y + 1, z + 1),
					FVector(x, y + 1, z + 1)
				};

				float isoLevel = 1;
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
				}

			}
		}
	}
}
