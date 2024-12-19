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
	bool Ricochet(class ABullet* Bullet, FHitResult Hit, float IncomingSpeed, float Reflectance);
	bool Penetrate(class ABullet* Bullet, FHitResult Hit, float IncomingSpeed);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(class ABullet* Bullet, FHitResult Hit);

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, Units="rad", ToolTip="The angle at which the probability is 1/2"))
		float RicochetThreshold = HALF_PI;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f))
		float RicochetThresholdSharpness = INFINITY;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, Units="rad"))
		float ShallowRandomRicochetAngleStandardDeviation = HALF_PI/18;
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, Units="rad"))
		float DeepRandomRicochetAngleStandardDeviation = HALF_PI/9;
	
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, UIMax = 1.f))
		float RicochetElasticity = 0.5f;

protected:
	virtual float GetReflectance(FVector Incoming, FVector Normal) const;
};
