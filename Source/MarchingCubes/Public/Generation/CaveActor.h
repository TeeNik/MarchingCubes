#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveActor.generated.h"

UCLASS()
class MARCHINGCUBES_API ACaveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaveActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FVector StartPoint;
	UPROPERTY(EditAnywhere)
	FVector EndPoint;
	UPROPERTY(EditAnywhere)
	float Radius;

};
