// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableActor.h"

#include "Firearms/Bullet.h"


// Sets default values
AHittableActor::AHittableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHittableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AHittableActor::HandleImpact(class ABullet* Bullet, FHitResult Hit) {
	if (bBounce && Bullet->Velocity.Length() > MinSpeed) {
		float Dot = Bullet->Velocity.GetSafeNormal().Dot(Hit.ImpactNormal);
		if (!FMath::IsNearlyEqual(Dot, 0, 1E-2) && -Dot < FMath::Cos(HALF_PI - MaxBounceAngle)) {
			if (Dot < 0) {
				FVector VelocityImpulse = Bullet->Velocity.ProjectOnToNormal(Hit.ImpactNormal);
				Bullet->Velocity -= 2 * VelocityImpulse;
			}
			Bullet->Velocity *= BounceSpeedMultiplier;
			OnHit(Bullet, Hit);
			return true;
		}
	}
	OnHit(Bullet, Hit);
	return false;
}



