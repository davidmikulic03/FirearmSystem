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

void ABullet::Move(float& DeltaSeconds, AActor* OriginIgnore) {
	SetActorRotation(Velocity.Rotation(), ETeleportType::ResetPhysics);
	FVector Acceleration = FVector::UpVector * GetWorld()->GetGravityZ();
	FVector StartLocation = GetActorLocation();

	auto NewIgnore = IgnoreActors;
	if (OriginIgnore)
		NewIgnore.Add(OriginIgnore);
	
	if (Penetrant) {
		FVector ProjectedVelocity = Velocity - DeltaSeconds * Velocity.GetSafeNormal() * DecelerationInMedium;
		if ((ProjectedVelocity | Velocity) < 0) ProjectedVelocity = FVector::ZeroVector;
		float MaxPenetrationTime = Velocity.Length() / DecelerationInMedium;
		FVector ProjectedLocation = GetActorLocation() + (ProjectedVelocity + Velocity / 2) * FMath::Min(MaxPenetrationTime, DeltaSeconds);

		FHitResult BackTraceHit;
		bool bBackTrace = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		ProjectedLocation,
		StartLocation,
		Collision->GetUnscaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, NewIgnore, EDrawDebugTrace::None, BackTraceHit,
		true, FLinearColor::Green, FLinearColor::Red,
		0.1f);
		
		if (bBackTrace) {
			float Distance = FVector::Distance(BackTraceHit.Location, StartLocation);
			float ProjectedDistance = FVector::Distance(StartLocation, ProjectedLocation);
			float Ratio = Distance / ProjectedDistance;
			float SqrtRatio = sqrt(Ratio);
			float TimePassed = SqrtRatio * DeltaSeconds;
			DeltaSeconds = DeltaSeconds - TimePassed;
			SetActorLocation(BackTraceHit.Location);
			auto temp = Penetrant;
			Penetrant = nullptr;
			Velocity -= Velocity.GetSafeNormal() * DecelerationInMedium * TimePassed;
			Move(DeltaSeconds, temp);
			return;
		} else {
			SetActorLocation(ProjectedLocation);
			Velocity = ProjectedVelocity;
			if (Velocity.SquaredLength() < 1.f) {
				OnInelasticCollision(ProjectedLocation);
				bIsMoving = false;
			}
		}
	}
	
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
	
	SetActorLocation(bHit ? Hit.Location + Collision->GetUnscaledSphereRadius() * Hit.Normal : NewLocation);

	// FTransform ComponentSpaceTransform = TestTrail->GetComponentTransform().Inverse();
	// TestTrail->SetStartPosition(ComponentSpaceTransform.TransformPosition(StartLocation));
	// TestTrail->SetEndPosition(ComponentSpaceTransform.TransformPosition(GetActorLocation()));
	// TestTrail->SetStartTangent(ComponentSpaceTransform.TransformVector(0.25f * Velocity));
	// TestTrail->SetEndTangent(ComponentSpaceTransform.TransformVector(0.25f * Velocity));
	
	if(bHit) {
		float Ratio = FVector::Distance(StartLocation, GetActorLocation()) / FVector::Distance(StartLocation, NewLocation);
		float NewDeltaSeconds = DeltaSeconds * Ratio;
		HandleImpact(Hit, NewDeltaSeconds);
	}
	
	Velocity += Acceleration * DeltaSeconds;
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
	DecelerationInMedium = Deceleration;
}

void ABullet::Tick(float DeltaSeconds) {
	if (bIsMoving)
		Move(DeltaSeconds, nullptr);
	// EvaluateTrail(DeltaSeconds);
	if (MaxLifetime<=0)
		return;
	if(LifetimeCounter < MaxLifetime)
		LifetimeCounter+=DeltaSeconds;
	else Destroy();
}

void ABullet::HandleImpact(FHitResult Hit, float DeltaSeconds) {
	if (auto a = Cast<IHittable>(Hit.GetActor())) {
		if (a->HandleImpact(this, Hit, DeltaSeconds)) {
			Move(DeltaSeconds, Hit.GetActor());
			return;
		}
	}
	Destroy();
	// Velocity = 0.2 * FMath::GetReflectionVector(Velocity, Hit.ImpactNormal);
	// SetActorLocation(Hit.Location);
	// if(auto f = Cast<AFirearmBase>(GetOwner()))
	// 	f->RegisterHit(Hit);
	// else Destroy();
}

