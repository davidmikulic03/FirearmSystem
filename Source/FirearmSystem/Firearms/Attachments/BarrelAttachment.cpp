#include "BarrelAttachment.h"

ABarrelAttachment::ABarrelAttachment() {
	FiringPoint = CreateDefaultSubobject<USceneComponent>("Firing Point");
	FiringPoint->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
}
