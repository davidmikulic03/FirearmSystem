#include "StockAttachment.h"

#include "FirearmSystem/Firearms/WeightedBodyContactPoint.h"

AStockAttachment::AStockAttachment() {

	Pivot = CreateDefaultSubobject<UWeightedBodyContactPoint>("Contact Point");
	Pivot->SetupAttachment(Mesh);
	PrimaryActorTick.bCanEverTick = false;
}
