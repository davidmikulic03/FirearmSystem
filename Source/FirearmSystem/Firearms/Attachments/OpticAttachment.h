#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OpticAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AOpticAttachment : public AActor {
	GENERATED_BODY()
public:
	AOpticAttachment();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float ZoomFactor = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		UMaterialInterface* PostProcessMaterial = nullptr;
};
