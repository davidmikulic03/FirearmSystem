#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BarrelAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API ABarrelAttachment : public AActor {
	GENERATED_BODY()
public:
	ABarrelAttachment();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float SpeedModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float VolumeMultiplier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f))
		float FlashMultiplier = 1.f;
};
