#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "BarrelAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API ABarrelAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	ABarrelAttachment();

	USceneComponent* GetBarrelExitLocation();
	float GetAccuracyModifier() {
		float Result = AccuracyModifier;
		if (bCanAttachFurther && BarrelAttachment)
			Result *= BarrelAttachment->GetAccuracyModifier();
		return Result;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* BarrelExitPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float AccuracyModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float SpeedModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float VolumeMultiplier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float FlashMultiplier = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment", meta=(EditCondition="bCanAttachFurther", EditConditionHides))
		ABarrelAttachment* BarrelAttachment = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanAttachFurther = false;
};
