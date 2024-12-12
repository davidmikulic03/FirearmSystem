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
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABullet::Fire(AFirearmBase* ShotFrom, FVector InVelocity) {
	SetOwner(ShotFrom);
	Velocity = 100 * InVelocity;
}

void ABullet::Tick(float DeltaTime) {
	FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
	FHitResult Hit;
	TArray<AActor*> Ignore;
	Ignore.Init(GetOwner(),1);
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		NewLocation,
		Collision->GetUnscaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false, Ignore, EDrawDebugTrace::ForDuration, Hit,
		true, FLinearColor::Green, FLinearColor::Red,
		0.2f);
	if(bHit) {
		SetActorLocation(Hit.ImpactPoint);
		Velocity = FVector::ZeroVector;
		if(auto f = Cast<AFirearmBase>(GetOwner()))
			f->RegisterHit(Hit.Component);
		Destroy();
	}
	else
		SetActorLocation(NewLocation);
	
	Velocity += FVector::UpVector * GetWorld()->GetGravityZ();
}

