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

bool AHittableActor::HandleImpact(class ABullet* Bullet, FHitResult Hit, float DeltaSeconds) {
	
	float RelativeSpeed = Bullet->Velocity | Hit.ImpactNormal;
	float Reflectance = GetReflectance(Bullet->Velocity.GetSafeNormal(), Hit.ImpactNormal);
	if (FMath::FRand() < Reflectance) 
		return Ricochet(Bullet, Hit, RelativeSpeed, Reflectance, DeltaSeconds);
	else
		return Penetrate(Bullet, Hit, DeltaSeconds);
}

bool AHittableActor::Ricochet(class ABullet* Bullet, FHitResult Hit, float IncomingSpeed, float Reflectance, float DeltaSeconds) {
	if (IncomingSpeed < 0) {
		float GaussianRadius = sqrt(-2*log(FMath::FRand()));
		float StandardDeviation = FMath::Lerp(DeepRandomRicochetAngleStandardDeviation, ShallowRandomRicochetAngleStandardDeviation, Reflectance);
		float RandomAngle = GaussianRadius * StandardDeviation;
		FVector RotationVector = RandomAngle * UKismetMathLibrary::RandomUnitVector();
		FVector RotationIntoGround = FVector::VectorPlaneProject(RotationVector, Hit.ImpactNormal);
		float AngleTowardGround = RotationIntoGround.Length();
		float CurrentAngle = asin(-Bullet->Velocity.GetSafeNormal() | Hit.ImpactNormal);
		if (AngleTowardGround > CurrentAngle)
			RotationVector -= 2 * RotationIntoGround;
		FQuat Rotation = FQuat::MakeFromRotationVector(RotationVector);
		
		FVector VelocityImpulse = IncomingSpeed * Hit.ImpactNormal;
		Bullet->Velocity -= 2 * VelocityImpulse;
		Bullet->Velocity = Rotation * Bullet->Velocity;
		Bullet->Velocity *= RicochetElasticity * Bullet->Elasticity;
	}
	if ((Bullet->Velocity | Hit.ImpactNormal) > 0) {
		OnRicochet(Bullet, Hit.ImpactPoint, Hit.ImpactNormal);
		return true;
	}
	else 
		return Penetrate(Bullet, Hit, DeltaSeconds);
}

bool AHittableActor::Penetrate(class ABullet* Bullet, FHitResult Hit, float DeltaSeconds) {
	if (!bPenetrate) {
		OnDestroy(Bullet, Hit.ImpactPoint, Hit.ImpactNormal);
		return false;
	}
	else if (!bPhysicalPenetrationParams) {
		bool bSuccess = FMath::FRand() < PenetrationProbability;
		if (!bSuccess)
			OnDestroy(Bullet, Hit.ImpactPoint, Hit.ImpactNormal);
		else
			OnPenetrate(Bullet, Hit.ImpactPoint, Hit.ImpactNormal);
		return bSuccess;
	}
	else {
		// float BulletDensity = Utility::GetDensityOf(Bullet->Material);
		// float SelfDensity = Utility::GetDensityOf(Material);
		// float DensityRatio = BulletDensity / SelfDensity;
		// float AveragePenetrationDepth = Bullet->BulletLength * DensityRatio;
		// float PenetrationDepth = sqrt(-2*log(FMath::FRand())) * AveragePenetrationDepth;
		// float Deceleration = Bullet->Velocity.SquaredLength() / (2 * PenetrationDepth);
		// float MaxPenetrationTime = Bullet->Velocity.Length() / Deceleration;
		// FVector ProjectedLocation = 0.5 * Bullet->Velocity * MaxPenetrationTime;
		
		Bullet->SetupPenetrationParams(this, 0);
		OnPenetrate(Bullet, Hit.ImpactPoint, Hit.ImpactNormal);
		return true;
	}
}

float AHittableActor::GetReflectance(FVector Incoming, FVector Normal) const {
	float Dot = (-Incoming) | Normal;
	float Angle = FMath::Acos(Dot);
	float Exp = 1+FMath::Exp(-RicochetThresholdSharpness * (Angle - RicochetThreshold)); 
	float Reflectance =  1/Exp;;
	return Reflectance;
}



