#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generation/ChunkSettings.h"
#include "MeshGenerator.generated.h"

class UProceduralMeshComponent;
class AChunk;

UCLASS()
class MARCHINGCUBES_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshGenerator();

	void AddPoint(AChunk* chunk, FVector hitPoint, bool isAddition);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FChunkSettings ChunkSettings;

	UPROPERTY(EditAnywhere)
	bool DrawDebugPoints = false;

	float MaxNoiseValue = 1.0f;
	UPROPERTY(EditAnywhere)
	bool DoGenerateMesh;

	UPROPERTY(Transient)
	TArray<AChunk*> Chunks;

};