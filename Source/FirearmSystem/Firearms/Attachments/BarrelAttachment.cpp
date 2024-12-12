#include "BarrelAttachment.h"

ABarrelAttachment::ABarrelAttachment() {
	BarrelExitPoint = CreateDefaultSubobject<USceneComponent>("Firing Point");
	BarrelExitPoint->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
}

FVector ABarrelAttachment::GetBarrelExitLocation() {
	return BarrelExitPoint->GetComponentLocation();
}
