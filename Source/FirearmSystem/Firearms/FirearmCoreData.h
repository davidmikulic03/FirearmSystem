#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "FirearmCoreData.generated.h"

UENUM()
enum EFiringType : uint8 {
	Automatic,
	SemiAutomatic,
	Manual
};

UCLASS(CollapseCategories)
class FIREARMSYSTEM_API UFirearmCoreData : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual")
		FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte<EFiringType> FiringType = Manual;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="FiringType!=EFiringType::Manual", EditConditionHides))
		bool bNeedsInitialCocking = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float BulletSpeed = INFINITY;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f,UIMax=1.f))
		float Accuracy = 1.f;

};
