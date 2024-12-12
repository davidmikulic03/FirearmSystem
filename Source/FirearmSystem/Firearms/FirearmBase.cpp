#include "FirearmBase.h"

#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/OpticAttachment.h"
#include "Attachments/StockAttachment.h"
#include "Attachments/UnderBarrelAttachment.h"

AFirearmBase::AFirearmBase() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;

	BarrelExitPoint = CreateDefaultSubobject<USceneComponent>("Barrel Attachment Point");
	BarrelExitPoint->SetupAttachment(Root);
	StockAttachmentPoint = CreateDefaultSubobject<USceneComponent>("Stock Attachment Point");
	StockAttachmentPoint->SetupAttachment(Root);
	OpticsAttachmentPoint = CreateDefaultSubobject<USceneComponent>("Optics Attachment Point");
	OpticsAttachmentPoint->SetupAttachment(Root);
	UnderBarrelAttachmentPoint = CreateDefaultSubobject<USceneComponent>("Under-barrel Attachment Point");
	UnderBarrelAttachmentPoint->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = true;
}

bool AFirearmBase::TryFire() {
	return false;
}

bool AFirearmBase::CanFire() {
	if(FirearmData && FireCounter <= 0)
		return true;
	return false;
}

void AFirearmBase::Tick(float DeltaSeconds) {
	if(FireCounter > 0)
		FireCounter-=DeltaSeconds;
}

void AFirearmBase::RegisterHit(TWeakObjectPtr<UPrimitiveComponent> Component) {
	if(Component.IsValid()) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("Hit actor %s"),
				*Component.Get()->GetOwner()->GetName()));
	}
}

FVector AFirearmBase::GetBarrelExitLocation() {
	if(!BarrelAttachment)
		return BarrelExitPoint->GetComponentLocation();
	else 
		return BarrelAttachment->GetBarrelExitLocation();
}

void AFirearmBase::AttachBarrel(ABarrelAttachment* InBarrel, ABarrelAttachment*& OutBarrel) {
	if (TryDetach(OutBarrel))
		OutBarrel = BarrelAttachment;
	if (TryAttach(InBarrel))
		BarrelAttachment = InBarrel;
}

void AFirearmBase::AttachStock(AStockAttachment* InStock, AStockAttachment*& OutStock) {
	if (TryDetach(OutStock))
		OutStock = StockAttachment;
	if (TryAttach(InStock))
		StockAttachment = InStock;
}

void AFirearmBase::AttachOptics(AOpticsAttachment* InOptics, AOpticsAttachment*& OutOptics) {
	if (TryDetach(OutOptics))
		OutOptics = OpticsAttachment;
	if (TryAttach(InOptics))
		OpticsAttachment = InOptics;
}

void AFirearmBase::AttachUnderBarrel(AUnderBarrelAttachment* InUnderBarrel, AUnderBarrelAttachment*& OutUnderBarrel) {
	if (TryDetach(OutUnderBarrel))
		OutUnderBarrel = UnderBarrelAttachment;
	if (TryAttach(InUnderBarrel)) {
		UnderBarrelAttachment = InUnderBarrel;
		
	}
}

bool AFirearmBase::TryAttach(AActor* InActor) {
	if (InActor) {
		InActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		if(InActor->IsA(ABarrelAttachment::StaticClass())) {
			InActor->SetActorLocation(BarrelExitPoint->GetComponentLocation());
			InActor->SetActorRotation(BarrelExitPoint->GetComponentRotation());
		}
		else if(InActor->IsA(AStockAttachment::StaticClass())) {
			InActor->SetActorLocation(StockAttachmentPoint->GetComponentLocation());
			InActor->SetActorRotation(StockAttachmentPoint->GetComponentRotation());
		}
		else if(InActor->IsA(AOpticsAttachment::StaticClass())) {
			InActor->SetActorLocation(OpticsAttachmentPoint->GetComponentLocation());
			InActor->SetActorRotation(OpticsAttachmentPoint->GetComponentRotation());
		}
		else if(InActor->IsA(AUnderBarrelAttachment::StaticClass())) {
			InActor->SetActorLocation(UnderBarrelAttachmentPoint->GetComponentLocation());
			InActor->SetActorRotation(UnderBarrelAttachmentPoint->GetComponentRotation());
		}
		return true;
	}
	return false;
}

bool AFirearmBase::TryDetach(AActor* InActor) {
	if (InActor) {
		InActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		return true;
	}
	return false;
}

void AFirearmBase::BeginPlay() {
	Super::BeginPlay();

	if(InitialBarrelAttachmentClass) {
		ABarrelAttachment* Discard = nullptr;
		AttachBarrel(
			Cast<ABarrelAttachment>(GetWorld()->SpawnActor(InitialBarrelAttachmentClass)),
			Discard);
	}
	if(InitialStockAttachmentClass) {
		AStockAttachment* Discard = nullptr;
		AttachStock(
			Cast<AStockAttachment>(GetWorld()->SpawnActor(InitialStockAttachmentClass)),
			Discard);
	}
	if(InitialOpticsAttachmentClass) {
		AOpticsAttachment* Discard = nullptr;
		AttachOptics(
			Cast<AOpticsAttachment>(GetWorld()->SpawnActor(InitialOpticsAttachmentClass)),
			Discard);
	}
	if(InitialUnderBarrelAttachmentClass) {
		AUnderBarrelAttachment* Discard = nullptr;
		AttachUnderBarrel(
			Cast<AUnderBarrelAttachment>(GetWorld()->SpawnActor(InitialUnderBarrelAttachmentClass)),
			Discard);
	}
}
