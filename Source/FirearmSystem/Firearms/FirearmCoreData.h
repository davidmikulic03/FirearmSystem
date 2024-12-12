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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="m/s"))
		float BulletSpeed = 400;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float Accuracy = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float FireFrequency = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=1))
		int ChamberCapacity = 1;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABullet> BulletClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="kg", UIMin=0.f))
		float Weight = 2.f;
};
