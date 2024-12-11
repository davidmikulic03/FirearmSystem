#include "StockAttachment.h"

AStockAttachment::AStockAttachment() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	AttachmentPoint = CreateDefaultSubobject<USceneComponent>("AttachmentPoint");
	AttachmentPoint->SetupAttachment(Mesh);

	PrimaryActorTick.bCanEverTick = false;
}
