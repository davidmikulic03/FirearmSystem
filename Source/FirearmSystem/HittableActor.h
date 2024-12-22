// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "ImpactMaterial.h"
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

	virtual bool HandleImpact(class ABullet* Bullet, FHitResult Hit, float DeltaSeconds) override;
	bool Ricochet(class ABullet* Bullet, FHitResult Hit, float IncomingSpeed, float Reflectance, float DeltaSeconds);
	bool Penetrate(class ABullet* Bullet, FHitResult Hit, float DeltaSeconds);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPenetrate(class ABullet* Bullet, FVector Location, FVector Normal);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRicochet(class ABullet* Bullet, FVector Location, FVector Normal);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDestroy(class ABullet* Bullet, FVector Location, FVector Normal);

	UPROPERTY(EditAnywhere, Category = "Impact Response|Ricochet", meta = (UIMin = 0.f, Units="rad", ToolTip="The angle at which the probability is 1/2"))
		float RicochetThreshold = HALF_PI;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Ricochet", meta = (UIMin = 0.f))
		float RicochetThresholdSharpness = INFINITY;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Ricochet", meta = (UIMin = 0.f, Units="rad"))
		float ShallowRandomRicochetAngleStandardDeviation = HALF_PI/18;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Ricochet", meta = (UIMin = 0.f, Units="rad"))
		float DeepRandomRicochetAngleStandardDeviation = HALF_PI/9;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Ricochet", meta = (UIMin = 0.f, UIMax = 1.f))
		float RicochetElasticity = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Impact Response|Penetration")
		bool bPenetrate = true;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Penetration", meta = (EditCondition="bPenetrate", EditConditionHides))
		bool bPhysicalPenetrationParams = true;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Penetration", meta = (UIMin=0.f, UIMax=1.f, EditCondition="bPhysicalPenetrationParams", EditConditionHides))
		float PenetrationDepthRandomness = 0.f;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Penetration", meta = (EditCondition="bPhysicalPenetrationParams", EditConditionHides))
		TEnumAsByte<EImpactMaterial> Material = Steel;
	UPROPERTY(EditAnywhere, Category = "Impact Response|Penetration", meta = (UIMin=0.f, UIMax=1.f, EditCondition="bPenetrate", EditConditionHides))
		float PenetrationProbability = 1.f;

	
	
protected:
	virtual float GetReflectance(FVector Incoming, FVector Normal) const;
};
