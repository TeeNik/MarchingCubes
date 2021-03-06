#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MARCHINGCUBES_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float FireTimeout = 0.2f;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void OnLMBPressed();
	void OnLMBReleased();
	bool IsLMBPressed = false;

	void OnRMBPressed();
	void OnRMBReleased();
	bool IsRMBPressed = false;

	float NextFireTime = 0.0f;

};
