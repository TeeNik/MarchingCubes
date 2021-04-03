#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class UProjectileMovementComponent;

UCLASS()
class MARCHINGCUBES_API ALightProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightProjectile();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly, Category = LightProjectile)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = LightProjectile)
	UPointLightComponent* Light;

	UPROPERTY(VisibleDefaultsOnly, Category = LightProjectile)
	UProjectileMovementComponent* ProjectileMovement;
};
