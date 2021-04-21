#pragma once

#include "ChunkSettings.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct MARCHINGCUBES_API FChunkSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumOfPoints = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IsoLevel = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CubeSize = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseScale = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FloorLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionValue = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionRadius = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WallsWidth = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
};
