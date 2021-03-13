// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshGenerator.generated.h"

class UProceduralMeshComponent;

UCLASS()
class MARCHINGCUBES_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshGenerator();

	void AddPoint(FVector hitPoint);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	int NumOfPoints = 10;
	UPROPERTY(EditAnywhere)
	float CubeSize = 100.0f;

	UPROPERTY(EditAnywhere)
	bool DrawDebugPoints = false;
	UPROPERTY(EditAnywhere)
	float SphereRadius = 150.0f;
	UPROPERTY(EditAnywhere)
	FVector SphereCenter;

private:
	void GenerateMesh();
	FVector InterpolateVertex(FVector4 a, FVector4 b);
	int IndexFromCoord(int x, int y, int z);
	TArray<FVector4> Points;
};