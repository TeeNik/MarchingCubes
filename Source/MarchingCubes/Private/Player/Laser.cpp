#include "Player/Laser.h"
#include "NiagaraComponent.h"

ALaser::ALaser()
{
	LaserParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserParticle"));
	RootComponent = LaserParticle;
	WeaponFireParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponFireParticle"));
	WeaponFireParticle->SetupAttachment(RootComponent);
	WeaponFireParticle->SetRelativeLocation(FVector::ZeroVector);
	HitParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitParticle"));
	HitParticle->SetupAttachment(RootComponent);
}

void ALaser::UpdateLaser(FVector start, FVector target)
{
	LaserParticle->SetVectorParameter(TEXT("User.BeamEnd"), target);
	HitParticle->SetVectorParameter(TEXT("HitLocation"), target);
}