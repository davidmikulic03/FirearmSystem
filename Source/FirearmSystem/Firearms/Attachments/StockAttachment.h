#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "StockAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AStockAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	AStockAttachment();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		USceneComponent* AttachmentPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(UIMin=0.f, UIMax=1.f))
		float RecoilMultiplier = 1.f;
};
