#include "Firearm.h"

#include "Bullet.h"
#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/OpticAttachment.h"
#include "Attachments/StockAttachment.h"
#include "Attachments/UnderBarrelAttachment.h"
#include "FirearmPivot.h"
#include "WeightedBodyContactPoint.h"
#include "FirearmSystem/Core/Gunslinger.h"
#include "Kismet/KismetMathLibrary.h"

AFirearm::AFirearm() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;
	Hand = CreateDefaultSubobject<UWeightedBodyContactPoint>("Pivot");
	Hand->SetupAttachment(Root);

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

bool AFirearm::TryFire() {
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

bool AFirearm::CanFire() {
	if(FirearmData && FireCounter <= 0)
		return true;
	return false;
}

void AFirearm::Tick(float DeltaSeconds) {
	if(FireCounter > 0)
		FireCounter-=DeltaSeconds;
}

void AFirearm::RegisterHit(FHitResult Hit) {
	auto Component = Hit.GetComponent();
	if(Component) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("Hit actor %s"),
				*Component->GetOwner()->GetName()));
	}
}

FVector AFirearm::GetBarrelExitLocation() {
	if(!BarrelAttachment)
		return BarrelExitPoint->GetComponentLocation();
	else 
		return BarrelAttachment->GetBarrelExitLocation();
}

void AFirearm::AttachBarrel(ABarrelAttachment* InBarrel, ABarrelAttachment*& OutBarrel) {
	if (TryDetach(OutBarrel))
		OutBarrel = BarrelAttachment;
	if (TryAttach(InBarrel))
		BarrelAttachment = InBarrel;
}

void AFirearm::AttachStock(AStockAttachment* InStock, AStockAttachment*& OutStock) {
	if (TryDetach(OutStock))
		OutStock = StockAttachment;
	if (TryAttach(InStock))
		StockAttachment = InStock;
}

void AFirearm::AttachOptics(AOpticsAttachment* InOptics, AOpticsAttachment*& OutOptics) {
	if (TryDetach(OutOptics))
		OutOptics = OpticsAttachment;
	if (TryAttach(InOptics))
		OpticsAttachment = InOptics;
}

void AFirearm::AttachUnderBarrel(AUnderBarrelAttachment* InUnderBarrel, AUnderBarrelAttachment*& OutUnderBarrel) {
	if (TryDetach(OutUnderBarrel))
		OutUnderBarrel = UnderBarrelAttachment;
	if (TryAttach(InUnderBarrel)) {
		UnderBarrelAttachment = InUnderBarrel;
		
	}
}

bool AFirearm::TryAttach(AActor* InActor) {
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

bool AFirearm::TryDetach(AActor* InActor) {
	if (InActor) {
		InActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		return true;
	}
	return false;
}

void AFirearm::EvaluateTruePivot() {
	if (!TruePivot) {
		TruePivot = Cast<UWeightedBodyContactPoint>(AddComponentByClass(UWeightedBodyContactPoint::StaticClass(), false, GetTransform(), false));
	}
	float WeightSum = Hand->LocationWeight;
	TruePivot->SetRelativeLocation(Hand->GetRelativeLocation() * Hand->LocationWeight);
	TruePivot->ResistParams = Hand->ResistParams;
	if (StockAttachment) {
		WeightSum += StockAttachment->Pivot->LocationWeight;
		TruePivot->ResistParams += StockAttachment->Pivot->ResistParams;
		TruePivot->AddRelativeLocation(StockAttachment->Pivot->GetRelativeLocation() * StockAttachment->Pivot->LocationWeight);
	}
	TruePivot->SetRelativeLocation(TruePivot->GetRelativeLocation() / WeightSum);
}

void AFirearm::RegisterImpulse(FVector Impulse) {
	if (auto p = Cast<UFirearmPivot>(GetRootComponent()->GetAttachParent())) {
		p->AddImpulse(Impulse, true);
	}
}

void AFirearm::BeginPlay() {
	Super::BeginPlay();

	UClass* BarrelClass = InitialBarrelAttachmentClass ? InitialBarrelAttachmentClass : FirearmData->DefaultBarrelClass;
	if(BarrelClass) {
		ABarrelAttachment* Discard = nullptr;
		AttachBarrel(
			GetWorld()->SpawnActor<ABarrelAttachment>(BarrelClass),
			Discard);
	}
	UClass* StockClass = InitialStockAttachmentClass ? InitialStockAttachmentClass : FirearmData->DefaultStockClass;
	if(StockClass) {
		AStockAttachment* Discard = nullptr;
		AttachStock(
			GetWorld()->SpawnActor<AStockAttachment>(StockClass),
			Discard);
	}
	if(InitialOpticsAttachmentClass) {
		AOpticsAttachment* Discard = nullptr;
		AttachOptics(
			GetWorld()->SpawnActor<AOpticsAttachment>(InitialOpticsAttachmentClass),
			Discard);
	}
	if(InitialUnderBarrelAttachmentClass) {
		AUnderBarrelAttachment* Discard = nullptr;
		AttachUnderBarrel(
			GetWorld()->SpawnActor<AUnderBarrelAttachment>(InitialUnderBarrelAttachmentClass),
			Discard);
	}
	EvaluateTruePivot();
}

float AFirearm::GetWeight() {
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
