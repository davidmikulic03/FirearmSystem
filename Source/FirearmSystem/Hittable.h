// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

UINTERFACE()
class UHittable : public UInterface
{
	GENERATED_BODY()
};

class FIREARMSYSTEM_API IHittable
{
	GENERATED_BODY()
public:
	virtual bool HandleImpact(class ABullet* Bullet, FHitResult Hit, float DeltaSeconds) = 0;
};
