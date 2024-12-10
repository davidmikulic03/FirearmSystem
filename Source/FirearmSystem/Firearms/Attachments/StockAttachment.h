#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StockAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AStockAttachment : public AActor {
	GENERATED_BODY()
public:
	AStockAttachment();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0.f, UIMax=1.f))
		float RecoilMultiplier = 1.f;
};
