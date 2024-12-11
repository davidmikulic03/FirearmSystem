#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "BarrelAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API ABarrelAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	ABarrelAttachment();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		USceneComponent* AttachmentPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float SpeedModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float VolumeMultiplier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float FlashMultiplier = 1.f;
};
