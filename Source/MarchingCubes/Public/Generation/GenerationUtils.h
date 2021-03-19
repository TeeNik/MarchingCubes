#pragma once

#include "CoreMinimal.h"

class MARCHINGCUBES_API GenerationUtils
{
public:

	static int Edges[12][2];
	static int EdgeTable[256];
	static int TriTable[256][16];

	static void DrawHalfCircle(const UWorld* InWorld, const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);
	static void DrawDebugTunnel(const UWorld* InWorld, FVector const& Center, float HalfHeight, float Radius, const FQuat& Rotation, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);
	static bool IsInsideCylider(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf);
	static bool IsInsideSphere(const FVector& center, const FVector& point, float radius);
	static bool IsInsideCapsule(const FVector& start, const FVector& end, const FVector& point, float radius, bool onlyHalf = false);

};
