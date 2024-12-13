#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FIREARMSYSTEM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

	virtual void Fire(class AFirearmBase* ShotFrom, FVector InVelocity);

	virtual void Move(float DeltaSeconds, AActor* OriginIgnore);

public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Collision;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, meta=(Units="kg"))
		float Weight = 0.07f;

	float MaxLifetime = 30.f;
	float LifetimeCounter = 0.f;

	FVector Velocity = FVector::ZeroVector;

protected:
	void HandleImpact(FHitResult Hit, float DeltaSeconds);

	TArray<AActor*> IgnoreActors;
};
