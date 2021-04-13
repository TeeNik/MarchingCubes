#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

class UProceduralMeshComponent;

UCLASS()
class MARCHINGCUBES_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunk();

	void Create(FVector origin, int numOfPoints, float noiseScale, float isoLevel, float cubeSize);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

private:
	static FVector InterpolateVertex(FVector4 a, FVector4 b, float isoLevel);
	int IndexFromCoord(int x, int y, int z) const;

	int NumOfPoints = 0;

	UPROPERTY()
	TArray<FVector4> Points;
};
