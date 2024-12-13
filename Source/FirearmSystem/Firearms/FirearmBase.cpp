#include "FirearmBase.h"

#include "Bullet.h"
#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/OpticAttachment.h"
#include "Attachments/StockAttachment.h"
#include "Attachments/UnderBarrelAttachment.h"
#include "FirearmSystem/FirearmPivot.h"
#include "Kismet/KismetMathLibrary.h"

AFirearmBase::AFirearmBase() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;
	Pivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	Pivot->SetupAttachment(Root);

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
	if(CanFire() && BulletsInChamber > 0) {
		const FVector Location = GetBarrelExitLocation();
		const FRotator Rotation = GetActorRotation();
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.Owner = this;
		if(auto Bullet = GetWorld()->SpawnActor<ABullet>(FirearmData->BulletClass,
			Location, Rotation, SpawnParameters))
		{
			float AverageErrorAngleInRadians = FMath::DegreesToRadians(FirearmData->Accuracy / 60);
			if (BarrelAttachment)
				BarrelAttachment->AccuracyModifier;
			FVector Direction = Root->GetForwardVector();
			
			FQuat RandomRotation = FQuat::MakeFromRotationVector(AverageErrorAngleInRadians * UKismetMathLibrary::RandomUnitVector());
			Direction = RandomRotation * Direction;
			FVector BulletVelocity = FirearmData->BulletSpeed * Direction;
			Bullet->Fire(this, BulletVelocity);
			FireCounter = 1.f/FirearmData->FireFrequency;
			BulletsInChamber--;
			FVector BulletMomentum = BulletVelocity * Bullet->Weight;
			FVector Impulse = -BulletMomentum / GetWeight();
			RegisterImpulse(Impulse);
			return true;
		}
	}
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

void AFirearmBase::RegisterHit(FHitResult Hit) {
	auto Component = Hit.GetComponent();
	if(Component) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("Hit actor %s"),
				*Component->GetOwner()->GetName()));
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
		FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepRelativeTransform;
		Rules.bWeldSimulatedBodies = true;
		auto Mesh = Cast<UStaticMeshComponent>(InActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (!Mesh)
			return false;
		Mesh->AttachToComponent(Root, Rules);
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

void AFirearmBase::RegisterImpulse(FVector Impulse) {
	if (auto c = Cast<UFirearmPivot>(GetRootComponent()->GetAttachParent())) {
		c->AddImpulse(Impulse);
	}
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

float AFirearmBase::GetWeight() {
	float Result = FirearmData->Weight;
	if (BarrelAttachment)
		Result+=BarrelAttachment->GetWeight();
	if (StockAttachment)
		Result+=StockAttachment->GetWeight();
	if (OpticsAttachment)
		Result+=OpticsAttachment->GetWeight();
	if (UnderBarrelAttachment)
		Result+=UnderBarrelAttachment->GetWeight();
	return Result;
}
