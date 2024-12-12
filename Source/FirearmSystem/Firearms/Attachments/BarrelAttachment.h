#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "BarrelAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API ABarrelAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	ABarrelAttachment();

	FVector GetBarrelExitLocation();

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
};
