#include "Firearm.h"

#include "FirearmCoreData.h"

AFirearm::AFirearm() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;
	BarrelAttachment = CreateDefaultSubobject<UChildActorComponent>("Barrel");
	BarrelAttachment->SetupAttachment(Root);
	OpticAttachment = CreateDefaultSubobject<UChildActorComponent>("Optics");
	OpticAttachment->SetupAttachment(Root);
	StockAttachment = CreateDefaultSubobject<UChildActorComponent>("Stock");
	StockAttachment->SetupAttachment(Root);
}
