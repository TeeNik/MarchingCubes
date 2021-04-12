#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "FastNoiseWrapper.h"
#include "MeshGenerator.generated.h"

class UProceduralMeshComponent;

UCLASS()
class MARCHINGCUBES_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshGenerator();

	void AddPoint(FVector hitPoint, bool isAddition);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FVector Bounds;
	UPROPERTY(EditAnywhere)
	int NumOfPoints = 10;
	UPROPERTY(EditAnywhere)
	float CubeSize = 100.0f;

	UPROPERTY(EditAnywhere)
	bool DrawDebugPoints = false;

	UPROPERTY(EditAnywhere)
	float AdditionRadius = 100.0f;
	UPROPERTY(EditAnywhere)
	float AdditionValue = 0.05f;

	UPROPERTY(EditAnywhere)
	float Persistence = 0.5f;
	UPROPERTY(EditAnywhere)
	float Lacunarity = 2.0f;
	UPROPERTY(EditAnywhere)
	float NoiseWeight = 1.0f;
	UPROPERTY(EditAnywhere)
	float NoiseScale = 100.0f;
	UPROPERTY(EditAnywhere)
	float WeightMultiplier = 1.0f;

	UPROPERTY(EditAnywhere)
	float Amplitude = 1.0f;
	UPROPERTY(EditAnywhere)
	float MaxNoiseValue = 1.0f;
	UPROPERTY(EditAnywhere)
	bool DoGenerateMesh;
	UPROPERTY(EditAnywhere)
	float IsoLevel = 0.5f;

private:
	void CreateChunk(FVector chunkOrigin);
};