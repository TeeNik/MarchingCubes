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

	FVector bounds = ChunkSettings.Bounds;
	for (int x = 0; x < bounds.X; ++x)
	{
		for (int y = 0; y < bounds.Y; ++y)
		{
			for (int z = 0; z < bounds.Z; ++z)
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
		bool result = FMath::SphereAABBIntersection(hitPoint,  FMath::Square(ChunkSettings.AdditionRadius), chunkBox);
		if (result)
		{
			chunk->AddPoint(hitPoint, isAddition);
		}
	}

	TArray<FHitResult> hitResults;
	ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(ChunkSettings.AdditionRadius);
	GetWorld()->SweepMultiByChannel(hitResults, hitPoint, hitPoint, FQuat::FQuat(), ECC, CollisionShape);
	
	for (const FHitResult& hitResult : hitResults)
	{
		if(hitResult.Actor->ActorHasTag(TEXT("Folliage")))
		{
			hitResult.Actor->Destroy();
		}
	}
}