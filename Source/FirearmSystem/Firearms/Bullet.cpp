// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "FirearmBase.h"
#include "Components/SphereComponent.h"
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

void ABullet::Fire(AFirearmBase* ShotFrom, FVector InVelocity) {
	SetOwner(ShotFrom);
	Velocity = 100 * InVelocity;
}

void ABullet::Move(float DeltaSeconds)
{
	FVector Acceleration = FVector::UpVector * GetWorld()->GetGravityZ();
	FVector NewLocation = GetActorLocation() + Velocity * DeltaSeconds + 0.5 * Acceleration * DeltaSeconds * DeltaSeconds;
	FHitResult Hit;
	TArray<AActor*> Ignore;
	if(GetOwner()) {
		GetOwner()->GetAttachedActors(Ignore);
		Ignore.Add(GetOwner());
		Ignore.Add(this);
	}
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		NewLocation,
		Collision->GetUnscaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, Ignore, EDrawDebugTrace::ForOneFrame, Hit,
		true, FLinearColor::Green, FLinearColor::Red,
		0.1f);
	if(bHit) {
		SetActorLocation(Hit.ImpactPoint);
		Velocity = FVector::ZeroVector;
		if(auto f = Cast<AFirearmBase>(GetOwner()))
			f->RegisterHit(Hit.Component);
		Destroy();
	}
	else
		SetActorLocation(NewLocation);
	
	Velocity += Acceleration * DeltaSeconds;

	if(LifetimeCounter < MaxLifetime)
		LifetimeCounter+=DeltaSeconds;
	else Destroy();
}

void ABullet::Tick(float DeltaSeconds) {
	Move(DeltaSeconds);
}

