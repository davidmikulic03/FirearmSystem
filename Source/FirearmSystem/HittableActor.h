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

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(class ABullet* Bullet, FHitResult Hit);

	UPROPERTY(EditAnywhere, meta = (Units="m/s"))
		float MaxIncomingVelocity = 0;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, UIMax = 1.f))
		float RicochetShallowProbability = 0.f;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, UIMax = 1.f))
		float RicochetDeepProbability = 0.f;
	
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, UIMax = 1.f))
		float RicochetElasticity = 0.5f;
	UPROPERTY(EditAnywhere, meta = (Units="rad"))
		float RicochetShallowRandomness = HALF_PI/9;
	UPROPERTY(EditAnywhere, meta = (Units="rad"))
		float RicochetDeepRandomness = HALF_PI/3;
};
