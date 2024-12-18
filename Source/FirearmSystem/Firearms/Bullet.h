﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FIREARMSYSTEM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

	virtual void Fire(class AFirearm* ShotFrom, FVector InVelocity);

	virtual void Move(float DeltaSeconds, AActor* OriginIgnore);

public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Collision;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, meta=(Units="kg"))
		float Weight = 0.07f;
	UPROPERTY(EditDefaultsOnly, meta=(UIMin=0.f, UIMax=1.f))
		float Elasticity = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta=(Units="s"))
		float MaxLifetime = 0;
	
	float LifetimeCounter = 0.f;

	FVector Velocity = FVector::ZeroVector;

protected:
	void HandleImpact(FHitResult Hit, float DeltaSeconds);

	TArray<AActor*> IgnoreActors;
};
