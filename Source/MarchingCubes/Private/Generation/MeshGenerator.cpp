#include "Generation/MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <DrawDebugHelpers.h>
#include "Generation/GenerationUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/CaveActor.h"
#include "Generation/Chunk.h"

AMeshGenerator::AMeshGenerator()
{
}

void AMeshGenerator::AddPoint(AChunk* chunk, FVector hitPoint, bool isAddition)
{
	/*
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			for(int k = -1; k <= 1; ++k)
			{
				int x = chunk->Origin.X + i;
				int y = chunk->Origin.Y + j;
				int z = chunk->Origin.Z + k;

				if((x >= 0 || x < Bounds.X) &&
					(y >= 0 || y < Bounds.Y) &&
					(z >= 0 || z < Bounds.Z))
				{
					
				}
			}
		}
	}
	*/
	for (AChunk* chunk : Chunks)
	{
		FVector min = chunk->Origin * (NumOfPoints - 1) * CubeSize;
		FVector max = (chunk->Origin + FVector::OneVector) * (NumOfPoints - 1) * CubeSize;
		FBox chunkBox(min, max);
		if(FMath::SphereAABBIntersection(hitPoint, AdditionRadius * AdditionRadius, chunkBox))
		{
			chunk->AddPoint(hitPoint, isAddition);
		}
	}
}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

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

				float noiseValue = 0;
				float frequency = 1;
				float amplitude = 1;

				//for (int i = 0; i < 4; i++)
				//{
				//	float v = FMath::PerlinNoise3D(point * NoiseScale * frequency);
				//	noiseValue += (v + 1) * .5f * amplitude;
				//	frequency *= 2;
				//	amplitude *= 0.5f;
				//}
				//point.W = noiseValue / 2 + height / 2;
				//UE_LOG(LogTemp, Log, TEXT("noise: %s"), *point.ToString());

				if(x == 0 || y == 0 || x >= Bounds.X - 2 || y >= Bounds.Y - 2 || z >= Bounds.Z - 2)
				{
					point.W = FMath::RandRange(0.15f, 0.25f);
				}

				if(point.Z < 3 && point.W > IsoLevel)
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

				//if (UKismetMathLibrary::IsPointInBox(scaledPoint, Bounds * CubeSize / 2, Bounds * CubeSize / 4))
				//{
				//	point.W = 0;
				//}
				//else
				//{
				//	point.W = 1;
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
			}
		}
	}

	for (int x = 0; x < Bounds.X; ++x)
	{
		for (int y = 0; y < Bounds.Y; ++y)
		{
			for (int z = 0; z < Bounds.Z; ++z)
			{
				FVector origin(x, y, z);
				FVector location = origin * (NumOfPoints - 1) * CubeSize;
				AChunk* chunk = GetWorld()->SpawnActor<AChunk>(AChunk::StaticClass(), location, FRotator::ZeroRotator);
				chunk->Create(origin, ChunkSettings);
				Chunks.Add(chunk);
			}
		}
	}
}