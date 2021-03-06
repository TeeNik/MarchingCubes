#include "Player/FPSCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Generation/MeshGenerator.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Generation/Chunk.h"
#include "Player/Laser.h"

AFPSCharacter::AFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	GetMesh()->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	GetMesh()->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	FP_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	AActor* meshGenerator = UGameplayStatics::GetActorOfClass(GetWorld(), AMeshGenerator::StaticClass());
	if(meshGenerator)
	{
		MeshGenerator = Cast<AMeshGenerator>(meshGenerator);
	}
}

void AFPSCharacter::Tick(float DeltaTime)
{
	if(IsValid(Laser))
	{
		FVector start = FP_MuzzleLocation->GetComponentLocation();
		FVector target = start + FirstPersonCameraComponent->GetForwardVector() * 1000;
		Laser->UpdateLaser(start, target);
	}

	if(IsRMBPressed && GetWorld()->GetTimeSeconds() > NextFireTime)
	{
		NextFireTime = GetWorld()->GetTimeSeconds() + FireTimeout;
		FHitResult hit;
		FVector start = FirstPersonCameraComponent->GetComponentLocation();
		FVector end = start + FirstPersonCameraComponent->GetForwardVector() * 10000;
		bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
		if (result)
		{
			TArray<FHitResult> hitResults;
			FVector location = hit.ImpactPoint;

			AChunk* chunk = Cast<AChunk>(hit.Actor);
			MeshGenerator->AddPoint(chunk, location, false);
		}
	}
}

void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AFPSCharacter::OnFire);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AFPSCharacter::OnRMBPressed);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AFPSCharacter::OnRMBReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFPSCharacter::OnRMBPressed()
{
	if(LaserBP)
	{
		Laser = Cast<ALaser>(GetWorld()->SpawnActor(LaserBP));
		Laser->AttachToComponent(FP_MuzzleLocation, FAttachmentTransformRules::KeepRelativeTransform);
		Laser->SetActorLocation(FP_MuzzleLocation->GetComponentLocation());
		IsRMBPressed = true;
	}
}

void AFPSCharacter::OnRMBReleased()
{
	if(IsValid(Laser) && IsRMBPressed)
	{
		IsRMBPressed = false;
		Laser->DestroyLaser();
	}
}

void AFPSCharacter::OnFire()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetControlRotation();
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}