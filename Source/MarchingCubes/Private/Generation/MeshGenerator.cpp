#include "Generation/MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <DrawDebugHelpers.h>
#include "Generation/GenerationUtils.h"


void DrawHalfCircleCustom(const UWorld* InWorld, const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	float	AngleDelta = 2.0f * (float)PI / ((float)NumSides);
	FVector	LastVertex = Base + X * Radius;

	for (int32 SideIndex = 0; SideIndex < (NumSides / 2); SideIndex++)
	{
		FVector	Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		DrawDebugLine(InWorld, LastVertex, Vertex, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		LastVertex = Vertex;
	}
}

void DrawDebugCapsuleCustom(const UWorld* InWorld, FVector const& Center, float HalfHeight, float Radius, const FQuat& Rotation, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		const int32 DrawCollisionSides = 16;

		FVector Origin = Center;
		FMatrix Axes = FQuatRotationTranslationMatrix(Rotation, FVector::ZeroVector);
		FVector XAxis = Axes.GetScaledAxis(EAxis::X);
		FVector YAxis = Axes.GetScaledAxis(EAxis::Y);
		FVector ZAxis = Axes.GetScaledAxis(EAxis::Z);

		// Draw top and bottom circles
		float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
		FVector TopEnd = Origin + HalfAxis * ZAxis;
		FVector BottomEnd = Origin - HalfAxis * ZAxis;

		DrawCircle(InWorld, TopEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawCircle(InWorld, BottomEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		// Draw domed caps
		DrawHalfCircleCustom(InWorld, TopEnd, YAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawHalfCircleCustom(InWorld, TopEnd, XAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		FVector NegZAxis = -ZAxis;

		DrawHalfCircleCustom(InWorld, BottomEnd, YAxis, NegZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawHalfCircleCustom(InWorld, BottomEnd, XAxis, NegZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		// Draw connected lines
		DrawDebugLine(InWorld, TopEnd + Radius * XAxis, BottomEnd + Radius * XAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawDebugLine(InWorld, TopEnd - Radius * XAxis, BottomEnd - Radius * XAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawDebugLine(InWorld, TopEnd + Radius * YAxis, BottomEnd + Radius * YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawDebugLine(InWorld, TopEnd - Radius * YAxis, BottomEnd - Radius * YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
	else
	{
		//UE_DRAW_SERVER_DEBUG_ON_EACH_CLIENT(DrawDebugCapsule, Center, HalfHeight, Radius, Rotation, AdjustColorForServer(Color), bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
}

void DrawDebugTunnelCustom(const UWorld* InWorld, FVector const& Center, float HalfHeight, float Radius, const FQuat& Rotation, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)
	{
		const int32 DrawCollisionSides = 16;

		FVector Origin = Center;
		FMatrix Axes = FQuatRotationTranslationMatrix(Rotation, FVector::ZeroVector);
		FVector XAxis = Axes.GetScaledAxis(EAxis::X);
		FVector YAxis = Axes.GetScaledAxis(EAxis::Y);
		FVector ZAxis = Axes.GetScaledAxis(EAxis::Z);

		// Draw top and bottom circles
		//float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
		float HalfAxis = HalfHeight;

		FVector TopEnd = Origin + HalfAxis * XAxis;
		FVector BottomEnd = Origin - HalfAxis * XAxis;

		DrawCircle(InWorld, TopEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawCircle(InWorld, BottomEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		// Draw domed caps
		DrawHalfCircleCustom(InWorld, TopEnd, XAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawHalfCircleCustom(InWorld, TopEnd, YAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		FVector NegZAxis = -ZAxis;

		DrawHalfCircleCustom(InWorld, BottomEnd, XAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawHalfCircleCustom(InWorld, BottomEnd, YAxis, ZAxis, Color, Radius, DrawCollisionSides, bPersistentLines, LifeTime, DepthPriority, Thickness);

		// Draw connected lines
		DrawDebugLine(InWorld, TopEnd + Radius * YAxis, BottomEnd + Radius * YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawDebugLine(InWorld, TopEnd - Radius * YAxis, BottomEnd - Radius * YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawDebugLine(InWorld, TopEnd + Radius * ZAxis, BottomEnd + Radius * ZAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		//DrawDebugLine(InWorld, TopEnd - Radius * YAxis, BottomEnd - Radius * YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
	else
	{
		//UE_DRAW_SERVER_DEBUG_ON_EACH_CLIENT(DrawDebugCapsule, Center, HalfHeight, Radius, Rotation, AdjustColorForServer(Color), bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
}

bool AMeshGenerator::IsInsideCylider(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf)
{
	//UE_LOG(LogTemp, Log, TEXT("%f %f"), point.X, point.Y, point.Z);
	FVector d = end - start;
	FVector pd = point - start;
	float lengthsq = FVector::DistSquared(start, end);

	float dot = FVector::DotProduct(pd, d);
	if( dot < 0.0f || dot > lengthsq)
	{
		return false;
	}

	if(onlyHalf)
	{
		FVector dir = d.GetSafeNormal();
		FVector p = start + dir * FMath::Sqrt(dot * dot / lengthsq);
		//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), p.X, p.Y, p.Z);
		if(point.Z < p.Z)
		{
			return false;
		}
	}
	
	float dsq = pd.SizeSquared() - dot * dot / lengthsq;
	return dsq <= radius*radius;
}

bool AMeshGenerator::IsInsideSphere(const FVector& center, const FVector& point, float radius)
{
	return FVector::DistSquared(center, point) <= radius * radius;
}

bool AMeshGenerator::IsInsideCapsule(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf /*= false*/)
{
	return IsInsideSphere(start, point, radius) || IsInsideSphere(end, point, radius) || IsInsideCylider(start, end, point, radius, onlyHalf);
}

AMeshGenerator::AMeshGenerator()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;

}

void AMeshGenerator::AddPoint(FVector hitPoint, bool isAddition)
{
	for (FVector4& point : Points)
	{
		if(FVector::Dist(point * CubeSize, hitPoint) < AdditionRadius)
		{
			point.W = isAddition ? 0.0f : 1.0f;
		}
	}
	GenerateMesh();
}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	FVector start(200, 200, 200);
	FVector end(600, 200, 400);

	//FQuat rot = FQuat::MakeFromEuler(FVector(-90, 0, 0));
	FQuat rot = (end - start).ToOrientationQuat();

	bool t = IsInsideCylider(start, end, FVector(400, 200, 300), 100, true);
	if(t)
	{
		end.X = 700;
	}

	const float radius = 100;
	DrawDebugTunnelCustom(GetWorld(), (start+end)/2, FVector::Dist(start, end)/2, radius, rot, FColor::Green, false, 20, -1, 2);
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 60, -1, 2);

	for (int x = 0; x < NumOfPoints; ++x)
	{
		for (int y = 0; y < NumOfPoints; ++y)
		{
			for (int z = 0; z < NumOfPoints; ++z)
			{
				FVector4 point(x, y, z, 1);
				FVector scaledPoint = point * CubeSize;

				//if(UKismetMathLibrary::IsPointInBox(scaledPoint, BoxOrigin, BoxExtent))
				if (IsInsideCapsule(start, end, scaledPoint, radius, true))
				{
					point.W = 0;
				}

				//float dist = FVector::DistSquared(scaledPoint, SphereCenter);
				//if (dist < SphereRadius * SphereRadius * 3)
				//{
				//	point.W = 0;
				//}

				if (DrawDebugPoints)
				{
					FColor color = point.W ? FColor::Green : FColor::Red;
					DrawDebugSphere(GetWorld(), scaledPoint, 2, 4, color, false, 25);
				}
				Points.Emplace(point);
			}
		}
	}
	GenerateMesh();

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
					tri.vertexA = InterpolateVertex(cubeCorners[a0], cubeCorners[b0]) * CubeSize;
					tri.vertexB = InterpolateVertex(cubeCorners[a1], cubeCorners[b1]) * CubeSize;
					tri.vertexC = InterpolateVertex(cubeCorners[a2], cubeCorners[b2]) * CubeSize;
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

FVector AMeshGenerator::InterpolateVertex(FVector4 a, FVector4 b)
{
	return a + (b - a) / 2;
}

int AMeshGenerator::IndexFromCoord(int x, int y, int z)
{
	return x + y * NumOfPoints + z * NumOfPoints * NumOfPoints;
}
