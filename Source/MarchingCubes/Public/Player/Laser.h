#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class MARCHINGCUBES_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaser();

	void UpdateLaser(FVector start, FVector target);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyLaser();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UNiagaraSystem* LaserTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* LaserParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* WeaponFireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* HitParticle;
};