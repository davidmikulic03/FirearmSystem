// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableActor.h"

#include "Firearms/Bullet.h"
#include "Kismet/KismetMathLibrary.h"


AHittableActor::AHittableActor()
{
	
	PrimaryActorTick.bCanEverTick = true;
}

void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHittableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AHittableActor::HandleImpact(class ABullet* Bullet, FHitResult Hit) {
	
	float RelativeSpeed = Bullet->Velocity | Hit.ImpactNormal;
	float VelocityRatio = (-RelativeSpeed / (MaxIncomingVelocity * 100));
	float Probability = FMath::Lerp(RicochetShallowProbability, RicochetDeepProbability, VelocityRatio) ;
	if (RicochetDeepProbability > 0 && FMath::FRand() < Probability && -RelativeSpeed < MaxIncomingVelocity * 100) {
		if (RelativeSpeed < 0) {
			FVector VelocityImpulse = Bullet->Velocity.ProjectOnToNormal(Hit.ImpactNormal);
			Bullet->Velocity -= 2 * VelocityImpulse;
		}
		Bullet->Velocity -= Hit.ImpactNormal * RelativeSpeed * RicochetElasticity * Bullet->Elasticity;
		float RandomAngle = FMath::Lerp(RicochetShallowRandomness, RicochetDeepRandomness, VelocityRatio * VelocityRatio);
		FVector RotationVector = RandomAngle * UKismetMathLibrary::RandomUnitVector();
		Bullet->Velocity = FQuat::MakeFromRotationVector(RotationVector) * Bullet->Velocity;
		if ((Bullet->Velocity | Hit.ImpactNormal) > 0) {
			OnHit(Bullet, Hit);
			return true;
		}
	}
	OnHit(Bullet, Hit);
	return false;
}



