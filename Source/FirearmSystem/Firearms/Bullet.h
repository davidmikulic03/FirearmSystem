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

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Collision;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	FVector Velocity = FVector::ZeroVector;
};
