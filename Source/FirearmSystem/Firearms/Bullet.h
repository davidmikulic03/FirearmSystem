#pragma once

#include "CoreMinimal.h"
#include "FirearmSystem/ImpactMaterial.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

USTRUCT()
struct FTrailEntry {
	GENERATED_BODY()
	FTrailEntry() { }
	FTrailEntry(FVector Position, double Time) : Position(Position), Time(Time) { }
	FVector Position = FVector::ZeroVector;
	double Time = 0.f;
};

UCLASS()
class FIREARMSYSTEM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

	UFUNCTION(BlueprintImplementableEvent)
		void OnInelasticCollision(FVector Location);
	
	virtual void Fire(class AFirearm* ShotFrom, FVector InVelocity);

	virtual void Move(float& DeltaSeconds);

	void EvaluateTrail(float DeltaSeconds);

	void SetupPenetrationParams(class AHittableActor* InActor, float Deceleration);

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
	UPROPERTY(EditDefaultsOnly, meta=(Units="cm"))
		float BulletLength = 1.f;
	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EImpactMaterial> Material = EImpactMaterial::Lead;
	
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
	
	class AHittableActor* Penetrant = nullptr;
	float DecelerationInMedium = 0.f;
protected:
	void HandleImpact(FHitResult Hit, float DeltaSeconds);

	void RegisterPosition();
	void PruneTrail();

	TArray<AActor*> IgnoreActors;
	
	TArray<FTrailEntry> PastPositions;
	double TimeOfFire = 0.0;

	bool bIsMoving = true;
};
