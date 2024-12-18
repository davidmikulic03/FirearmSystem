// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecoilResistParams.h"
#include "Components/SceneComponent.h"
#include "WeightedContactPoint.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIREARMSYSTEM_API UWeightedContactPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	UWeightedContactPoint();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm", meta=(UIMin=0.f))
		float LocationWeight = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firearm")
		FRecoilResistParams ResistParams;
};
