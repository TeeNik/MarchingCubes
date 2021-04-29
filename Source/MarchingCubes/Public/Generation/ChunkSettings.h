#pragma once

#include "ChunkSettings.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct MARCHINGCUBES_API FChunkSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Bounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumOfPoints = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IsoLevel = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CubeSize = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseScale = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FloorLevel = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionValue = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionRadius = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WallsWidth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
};
