#include "Generation/CaveActor.h"
#include "Generation/GenerationUtils.h"

ACaveActor::ACaveActor()
{
}

void ACaveActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACaveActor::Draw() const
{
	FQuat rot = (EndPoint - StartPoint).ToOrientationQuat();
	GenerationUtils::DrawDebugTunnel(GetWorld(), (StartPoint + EndPoint) / 2, FVector::Dist(StartPoint, EndPoint) / 2, TunnelRadius, rot, FColor::Green, false, 20, -1, 2);
}