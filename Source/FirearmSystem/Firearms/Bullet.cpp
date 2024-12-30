// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Firearm.h"
#include "Components/SphereComponent.h"
#include "Components/SplineMeshComponent.h"
#include "FirearmSystem/Hittable.h"
#include "FirearmSystem/HittableActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/MovieSceneTracks/Private/MovieSceneTracksCustomAccessors.h"


ABullet::ABullet() {

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABullet::Fire(AFirearm* ShotFrom, FVector InVelocity) {
	SetOwner(ShotFrom);
	Velocity = 100 * InVelocity;
	if(GetOwner()) {
		GetOwner()->GetAttachedActors(IgnoreActors);
		IgnoreActors.Add(GetOwner()->GetAttachParentActor());
		IgnoreActors.Add(GetOwner());
		IgnoreActors.Add(this);
	}
	if (TrailTime>0) {
		
		Trail.Reserve(MaxTrailMeshes);
		for (int i = 0; i < MaxTrailMeshes; ++i) {
		
			if (auto m = Cast<USplineMeshComponent>(AddComponentByClass(
			USplineMeshComponent::StaticClass(),
			true,
			GetTransform(),
			false))) {
				m->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				m->SetVisibility(false);
				m->SetMobility(EComponentMobility::Movable);
				m->SetStaticMesh(TrailMesh);
				Trail.Add(m);
			}
		}
	}
}

void ABullet::Move(float& DeltaSeconds, AActor*& OriginIgnore) {
	if (DeltaSeconds <=0)
		return;
	SetActorRotation(Velocity.Rotation(), ETeleportType::ResetPhysics);
	FVector Acceleration = FVector::UpVector * GetWorld()->GetGravityZ();
	FVector StartLocation = GetActorLocation();

	auto NewIgnore = IgnoreActors;
	if (OriginIgnore)
		NewIgnore.Add(OriginIgnore);
	
	if (Penetrant) {

		float BulletDensity = Utility::GetDensityOf(Material);
		float SelfDensity = Penetrant->bUseMaterials ? Utility::GetDensityOf(Penetrant->Material) : Penetrant->Density;
		float DensityRatio = BulletDensity / SelfDensity;
		float AveragePenetrationDepth = BulletLength * DensityRatio;
		float Rand = Penetrant->PenetrationDepthRandomness * (sqrt(-2*log(FMath::FRand())) * AveragePenetrationDepth - AveragePenetrationDepth / 2);
		float PenetrationDepth = AveragePenetrationDepth + Rand;
		float Deceleration = (40000*40000) / (2 * PenetrationDepth);
		
		float Speed = Velocity.Size();
		float MaxPenetrationTime = Speed / Deceleration;
		float MinDtPen = FMath::Min(MaxPenetrationTime, DeltaSeconds);
		FVector ProjectedVelocity = Velocity.GetClampedToMaxSize(Speed - MinDtPen * Deceleration);
		if ((ProjectedVelocity | Velocity) < 0) ProjectedVelocity = FVector::ZeroVector;
		FVector ProjectedLocation = GetActorLocation() + Velocity * MinDtPen - 0.5 * Deceleration * (Velocity / Speed) * MinDtPen * MinDtPen;

		TArray<FHitResult> BackTraceHits;
		bool bBackTrace = UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		ProjectedLocation,
		StartLocation,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, NewIgnore, EDrawDebugTrace::None, BackTraceHits,
		true, FLinearColor::Green, FLinearColor::Red,
		0.1f);

		FHitResult* Hit = bBackTrace ? BackTraceHits.FindByPredicate([this](FHitResult h) {
				return h.GetActor() == Penetrant;
			}) : nullptr;
		if (Hit && Hit->Distance > 0) {
			float ProjectedDistance = FVector::Distance(StartLocation, ProjectedLocation);
			float Ratio = Hit->Distance / ProjectedDistance;
			float SqrtRatio = sqrt(Ratio);
			float TimePassed = SqrtRatio * DeltaSeconds;
			DeltaSeconds = DeltaSeconds - TimePassed;
			SetActorLocation(Hit->Location);
			OriginIgnore = Penetrant;
			Penetrant = nullptr;
			Velocity -= Velocity.GetSafeNormal() * DecelerationInMedium * TimePassed;
		} else {
			SetActorLocation(ProjectedLocation);
			Velocity = ProjectedVelocity;
			DeltaSeconds = DeltaSeconds - MinDtPen;
			if (Velocity.SquaredLength() < 1.f) {
				OnInelasticCollision(ProjectedLocation);
				bIsMoving = false;
			}
		}
	}
	else {
		FVector NewLocation = GetActorLocation() + Velocity * DeltaSeconds + 0.5 * Acceleration * DeltaSeconds * DeltaSeconds;
		FHitResult Hit;

		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartLocation,
			NewLocation,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false, NewIgnore, EDrawDebugTrace::None, Hit,
			true, FLinearColor::Green, FLinearColor::Red,
			0.1f);
		// if (bHit && Hit.Distance>0) {
		// 	OnInelasticCollision(Hit.Location);
		// 	bIsMoving = false;
		// }
	
		SetActorLocation(bHit ? Hit.Location + Collision->GetUnscaledSphereRadius() * Hit.Normal : NewLocation);
	
		if(bHit) {
			float Ratio = FVector::Distance(StartLocation, GetActorLocation()) / FVector::Distance(StartLocation, NewLocation);
			DeltaSeconds *= Ratio;
			HandleImpact(Hit, DeltaSeconds);
		}
		else DeltaSeconds = 0;
	
		Velocity += Acceleration * DeltaSeconds;
	}
}

void ABullet::EvaluateTrail(float DeltaSeconds)
{
	if (Trail.IsEmpty())
		return;
	
	float Speed = Velocity.Length();
	float MaxTrailLength = Speed * TrailTime;
	float CurrentTrailLength = 0.f;
	float TrailArrayLength = 0.f;
	
	// for (int i = Trail.Num()-1; i > 0; --i) {
	// 	Trail[i]->SetVisibility(true);
	// 	Trail[i]->SplineParams.EndPos = Trail[i]->SplineParams.StartPos;
	// 	Trail[i]->SplineParams.StartPos = Trail[i-1]->SplineParams.StartPos;
	// }
	// Trail[0]->SplineParams.EndPos = Trail[0]->SplineParams.StartPos;
	// Trail[0]->SplineParams.StartPos = GetActorLocation();
	// Trail[0]->SplineParams.EndTangent = Trail[0]->SplineParams.StartTangent;
	// Trail[0]->SplineParams.StartTangent = Velocity / Speed;
	
	// bool bVisible = true;
	// for (int i = 0; i < Trail.Num()-1; ++i) {
	// 	Trail[i]->SetVisibility(bVisible);
	// 	if (!bVisible)
	// 		continue;
	// 	
	// 	FVector End = Trail[i+1]->SplineParams.StartPos;
	// 	Trail[i+1]->SplineParams.StartPos = Trail[i]->SplineParams.StartPos;
	// 	Trail[i]->SplineParams.EndPos = End;
	// 	
	// 	// FVector Offset = Trail[i]->SplineParams.EndPos - Trail[i]->SplineParams.StartPos;
	// 	// CurrentTrailLength += Offset.Length();
	// 	// float NewLength = CurrentTrailLength - MaxTrailLength;
	// 	// CurrentTrailLength = MaxTrailLength;
	// 	// if (CurrentTrailLength > MaxTrailLength) {
	// 	// 	bVisible = false;
	// 	// }
	// }
}

void ABullet::SetupPenetrationParams(class AHittableActor* InActor, float Deceleration) {
	Penetrant = InActor;
}

void ABullet::Tick(float DeltaSeconds) {
	if(MaxLifetime!=0 && LifetimeCounter < MaxLifetime)
		LifetimeCounter+=DeltaSeconds;
	else if (MaxLifetime!=0) Destroy();

	AActor* Ignore = nullptr;
	while (bIsMoving && DeltaSeconds>0)
		Move(DeltaSeconds, Ignore);
}

void ABullet::HandleImpact(FHitResult Hit, float DeltaSeconds) {
	if (auto a = Cast<IHittable>(Hit.GetActor())) {
		if (a->HandleImpact(this, Hit, DeltaSeconds))
			return;
	}
	Destroy();
	// Velocity = 0.2 * FMath::GetReflectionVector(Velocity, Hit.ImpactNormal);
	// SetActorLocation(Hit.Location);
	// if(auto f = Cast<AFirearmBase>(GetOwner()))
	// 	f->RegisterHit(Hit);
	// else Destroy();
}

