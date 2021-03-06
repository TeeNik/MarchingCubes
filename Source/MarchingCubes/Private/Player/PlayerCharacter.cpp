#include "Player/PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Generation/MeshGenerator.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if((IsLMBPressed || IsRMBPressed) && GetWorld()->GetTimeSeconds() > NextFireTime)
	{
		NextFireTime = GetWorld()->GetTimeSeconds() + FireTimeout;

		FHitResult hit;
		FVector start = GetActorLocation();
		FVector end = start + GetActorForwardVector() * 10000;
		bool result = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
		if (result)
		{
			AMeshGenerator* generator = Cast<AMeshGenerator>(hit.Actor);
			if (generator)
			{
				//generator->AddPoint(hit.ImpactPoint, IsLMBPressed);
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacter::OnLMBPressed);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacter::OnLMBReleased);

	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &APlayerCharacter::OnRMBPressed);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &APlayerCharacter::OnRMBReleased);
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::OnLMBPressed()
{
	IsLMBPressed = true;
}

void APlayerCharacter::OnLMBReleased()
{
	IsLMBPressed = false;
}

void APlayerCharacter::OnRMBPressed()
{
	IsRMBPressed = true;
}

void APlayerCharacter::OnRMBReleased()
{
	IsRMBPressed = false;
}
