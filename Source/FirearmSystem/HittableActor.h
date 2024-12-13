// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "GameFramework/Actor.h"

#include "HittableActor.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AHittableActor : public AActor, public IHittable
{
	GENERATED_BODY()

public:
	AHittableActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool HandleImpact(class ABullet* Bullet, FHitResult Hit) override;
};
