#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "OpticAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AOpticAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	AOpticAttachment();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		USceneComponent* AttachmentPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		float ZoomFactor = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f))
		UMaterialInterface* PostProcessMaterial = nullptr;
};
