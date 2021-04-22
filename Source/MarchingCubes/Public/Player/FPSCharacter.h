#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UParticleSystem;
class UParticleSystemComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class AMeshGenerator;

UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UNiagaraSystem* LaserTemplate;

	UPROPERTY(EditAnywhere)
	float FireTimeout = 0.2f;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* LaserParticle;

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyLaser();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDestroyingLaser = false;

private:

	void OnFire();
	void OnRMBPressed();
	void OnRMBReleased();

	void MoveForward(float Val);
	void MoveRight(float Val);

	bool IsRMBPressed = false;
	float NextFireTime = 0.0f;

	UPROPERTY(Transient)
	AMeshGenerator* MeshGenerator;
};

