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

	UFUNCTION(BlueprintCallable)
	void Draw() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TunnelRadius;



};
