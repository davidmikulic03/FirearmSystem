// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletManager.generated.h"

UCLASS()
class FIREARMSYSTEM_API ABulletManager : public AActor
{
	GENERATED_BODY()

public:
	ABulletManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	static TArray<class ABullet*> Bullets;
};
