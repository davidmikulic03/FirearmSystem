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

	virtual void Fire(class AFirearm* ShotFrom, FVector InVelocity);

	virtual void Move(float DeltaSeconds, AActor* OriginIgnore);

	void EvaluateTrail(float DeltaSeconds);

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
	UPROPERTY(EditDefaultsOnly, meta=(Units="s"))
		float TrailTime = 0.05f;
	UPROPERTY(EditDefaultsOnly, meta=(UIMin=0, UIMax=10.f, EditCondition="TrailTime>0", EditConditionHides))
		int MaxTrailMeshes = 1;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="TrailTime>0", EditConditionHides))
		UStaticMesh* TrailMesh;
	
	float LifetimeCounter = 0.f;

	UPROPERTY(BlueprintReadOnly)
		FVector Velocity = FVector::ZeroVector;

	UPROPERTY()
		TArray<class USplineMeshComponent*> Trail;
	
protected:
	void HandleImpact(FHitResult Hit, float DeltaSeconds);

	TArray<AActor*> IgnoreActors;
};
