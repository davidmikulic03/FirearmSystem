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

	UPROPERTY(EditAnywhere)
		bool bBounce = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = bBounce, EditConditionHides, Units="rad"))
		float MaxBounceAngle = 0;
	UPROPERTY(EditAnywhere, meta = (EditCondition = bBounce, Units="m/s", EditConditionHides))
		float MinSpeed = 100;
	UPROPERTY(EditAnywhere, meta = (EditCondition = bBounce, EditConditionHides))
		float BounceSpeedMultiplier = 0.9;
};
