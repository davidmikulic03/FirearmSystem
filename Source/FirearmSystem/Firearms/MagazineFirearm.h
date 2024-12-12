// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirearmBase.h"
#include "MagazineFirearm.generated.h"

UCLASS()
class FIREARMSYSTEM_API AMagazineFirearm : public AFirearmBase
{
	GENERATED_BODY()

public:
	AMagazineFirearm();

	virtual bool TryFire() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class AMagazine* Magazine;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* MagazineAttachmentPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMagazine> MagazineClass;

protected:
	virtual void BeginPlay() override;

	void ReplaceMagazine();

	virtual float GetWeight() override;

public:
	virtual void Tick(float DeltaTime) override;
};
