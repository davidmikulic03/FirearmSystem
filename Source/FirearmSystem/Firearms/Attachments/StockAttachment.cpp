#include "StockAttachment.h"

#include "FirearmSystem/Firearms/WeightedContactPoint.h"

AStockAttachment::AStockAttachment() {

	Pivot = CreateDefaultSubobject<UWeightedContactPoint>("Contact Point");
	Pivot->SetupAttachment(Mesh);
	PrimaryActorTick.bCanEverTick = false;
}
