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
class AMeshGenerator;
class ALaser;

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

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TSubclassOf<ALaser> LaserBP;
	UPROPERTY(EditAnywhere)
	float FireTimeout = 0.2f;
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	ALaser* Laser;

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

