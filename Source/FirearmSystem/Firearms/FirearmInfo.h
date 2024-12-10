// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FirearmInfo.generated.h"

UENUM()
enum EFiringType : uint8 {
	Automatic,
	SemiAutomatic,
	Manual
};

UCLASS(CollapseCategories)
class FIREARMSYSTEM_API UFirearmInfo : public UDataAsset {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual")
		class UStaticMesh* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual")
		FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
		bool bHasMagazine = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0, EditCondition="bHasMagazine==true"), EditConditionHides)
		int MagazineCapacity = 6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EFiringType FiringType = Manual;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="FiringType!=EFiringType::Manual"))
		bool bInitiallyCocked = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float BulletSpeed = INFINITY;
};
