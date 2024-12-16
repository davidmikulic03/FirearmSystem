// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Firearm.h"
#include "Components/SphereComponent.h"
#include "FirearmSystem/Hittable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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
}

void ABullet::Move(float DeltaSeconds, AActor* OriginIgnore)
{
	SetActorRotation(Velocity.Rotation(), ETeleportType::ResetPhysics);
	FVector Acceleration = FVector::UpVector * GetWorld()->GetGravityZ();
	FVector NewLocation = GetActorLocation() + Velocity * DeltaSeconds + 0.5 * Acceleration * DeltaSeconds * DeltaSeconds;
	FHitResult Hit;

	auto NewIgnore = IgnoreActors;
	NewIgnore.Add(OriginIgnore);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		NewLocation,
		Collision->GetUnscaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, NewIgnore, EDrawDebugTrace::None, Hit,
		true, FLinearColor::Green, FLinearColor::Red,
		0.1f);
	if(bHit) {
		float NewDeltaSeconds = DeltaSeconds * FVector::Distance(GetActorLocation(), NewLocation) / Velocity.Length();
		SetActorLocation(Hit.Location);
		HandleImpact(Hit, NewDeltaSeconds);
	}
	else
		SetActorLocation(NewLocation);
	
	Velocity += Acceleration * DeltaSeconds;

	
}

void ABullet::Tick(float DeltaSeconds) {
	Move(DeltaSeconds, nullptr);
	if (MaxLifetime<=0)
		return;
	if(LifetimeCounter < MaxLifetime)
		LifetimeCounter+=DeltaSeconds;
	else Destroy();
}

void ABullet::HandleImpact(FHitResult Hit, float DeltaSeconds) {
	if (auto a = Cast<IHittable>(Hit.GetActor())) {
		if (a->HandleImpact(this, Hit))
			Move(DeltaSeconds, Hit.GetActor());
		else Destroy();
	}
	else
		Destroy();
	// Velocity = 0.2 * FMath::GetReflectionVector(Velocity, Hit.ImpactNormal);
	// SetActorLocation(Hit.Location);
	// if(auto f = Cast<AFirearmBase>(GetOwner()))
	// 	f->RegisterHit(Hit);
	// else Destroy();
}

