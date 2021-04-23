#include "Generation/MeshGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/Chunk.h"
#include "DrawDebugHelpers.h"

AMeshGenerator::AMeshGenerator()
{
}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int x = 0; x < Bounds.X; ++x)
	{
		for (int y = 0; y < Bounds.Y; ++y)
		{
			for (int z = 0; z < Bounds.Z; ++z)
			{
				FVector origin(x, y, z);
				FVector location = origin * (ChunkSettings.NumOfPoints - 1) * ChunkSettings.CubeSize;
				AChunk* chunk = GetWorld()->SpawnActor<AChunk>(AChunk::StaticClass(), location, FRotator::ZeroRotator);
				chunk->Create(origin, ChunkSettings);
				Chunks.Add(chunk);
			}
		}
	}
}

void AMeshGenerator::AddPoint(AChunk* c, FVector hitPoint, bool isAddition)
{
	for (AChunk* chunk : Chunks)
	{
		FVector min = chunk->Origin * (ChunkSettings.NumOfPoints - 1) * ChunkSettings.CubeSize;
		FVector max = (chunk->Origin + FVector::OneVector) * (ChunkSettings.NumOfPoints - 1) * ChunkSettings.CubeSize;
		FBox chunkBox(min, max);
		bool result = FMath::SphereAABBIntersection(hitPoint, AdditionRadius * AdditionRadius, chunkBox);
		if (result)
		{
			chunk->AddPoint(hitPoint, isAddition);
		}
	}
}