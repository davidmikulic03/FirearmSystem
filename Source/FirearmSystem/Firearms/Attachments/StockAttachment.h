#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"

#include "StockAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AStockAttachment : public AFirearmAttachment {
	GENERATED_BODY()
public:
	AStockAttachment();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UWeightedContactPoint* Pivot;
};
