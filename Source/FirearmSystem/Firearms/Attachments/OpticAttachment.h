#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "OpticAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AOpticsAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	AOpticsAttachment();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float ZoomFactor = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		UMaterialInterface* PostProcessMaterial = nullptr;
};
