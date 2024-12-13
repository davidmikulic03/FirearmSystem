// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableActor.h"


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
	return false;
}



