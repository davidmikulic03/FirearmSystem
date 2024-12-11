#include "Firearm.h"

#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/OpticAttachment.h"
#include "Attachments/StockAttachment.h"

AFirearm::AFirearm() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;

	PrimaryActorTick.bCanEverTick = true;
}

void AFirearm::AttachBarrel(ABarrelAttachment* InBarrel, ABarrelAttachment*& OutBarrel) {
	if (TryAttach(InBarrel))
		BarrelAttachment = InBarrel;
	if (TryDetach(OutBarrel))
		OutBarrel = BarrelAttachment;
}

void AFirearm::AttachStock(AStockAttachment* InStock, AStockAttachment*& OutStock) {
	if (TryAttach(InStock))
		StockAttachment = InStock;
	if (TryDetach(OutStock))
		OutStock = StockAttachment;
}

void AFirearm::AttachOptics(AOpticAttachment* InOptics, AOpticAttachment*& OutOptics) {
	if (TryAttach(InOptics))
		OpticAttachment = InOptics;
	if (TryDetach(OutOptics))
		OutOptics = OpticAttachment;
}

bool AFirearm::TryAttach(AActor* InActor) {
	if (InActor) {
		InActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		return true;
	}
	return false;
}

bool AFirearm::TryDetach(AActor* InActor) {
	if (InActor) {
		InActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		return true;
	}
	return false;
}
