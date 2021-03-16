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

	void AddPoint(FVector hitPoint, bool isAddition);

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
	float AdditionRadius = 100.0f;

	UPROPERTY(EditAnywhere)
	float SphereRadius = 150.0f;
	UPROPERTY(EditAnywhere)
	FVector SphereCenter;
	UPROPERTY(EditAnywhere)
	FVector BoxOrigin;
	UPROPERTY(EditAnywhere)
	FVector BoxExtent;

private:
	void GenerateMesh();
	FVector InterpolateVertex(FVector4 a, FVector4 b);
	int IndexFromCoord(int x, int y, int z);
	TArray<FVector4> Points;

	bool IsInsideCylider(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf);
	bool IsInsideSphere(const FVector& center, const FVector& point, float radius);
	bool IsInsideCapsule(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf = false);

};