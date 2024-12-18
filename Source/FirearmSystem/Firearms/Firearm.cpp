#include "Firearm.h"

#include "Bullet.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/StockAttachment.h"
#include "FirearmPivot.h"
#include "WeightedContactPoint.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "FirearmSystem/Core/Gunslinger.h"
#include "Kismet/KismetMathLibrary.h"

AFirearm::AFirearm() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;
	Hand = CreateDefaultSubobject<UWeightedContactPoint>("Pivot");
	Hand->SetupAttachment(Root);

	BarrelExitPoint = CreateDefaultSubobject<USceneComponent>("Barrel Attachment Point");
	BarrelExitPoint->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = true;
}

bool AFirearm::TryFire() {
	if(CanFire() && BulletsInChamber > 0) {
		const FVector Location = GetBarrelExit()->GetComponentLocation();
		const FRotator Rotation = GetActorRotation();
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.Owner = this;
		if(auto Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass,
			Location, Rotation, SpawnParameters))
		{
			float AverageErrorAngleInRadians = FMath::DegreesToRadians(Accuracy / 60);
			// if (auto c = GetAttachment<ABarrelAttachment>())
			// 	Accuracy *= c->AccuracyModifier;
			FVector Direction = Root->GetForwardVector();
			
			FQuat RandomRotation = FQuat::MakeFromRotationVector(AverageErrorAngleInRadians * UKismetMathLibrary::RandomUnitVector());
			Direction = RandomRotation * Direction;
			FVector BulletVelocity = BulletSpeed * Direction;
			Bullet->Fire(this, BulletVelocity);
			FireCounter = 1.f/FireFrequency;
			BulletsInChamber--;
			FVector BulletMomentum = BulletVelocity * Bullet->Weight;
			FVector Impulse = -BulletMomentum / GetWeight();
			RegisterImpulse(Impulse);
			if (auto s = GetMuzzleFlashSystem()) {
				UNiagaraFunctionLibrary::SpawnSystemAttached(s, GetBarrelExit(), NAME_None,
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
			}
			return true;
		}
	}
	return false;
}

bool AFirearm::CanFire() {
	if(FireCounter <= 0)
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

USceneComponent* AFirearm::GetBarrelExit() {
	ABarrelAttachment* BarrelAttachment = nullptr;
	for (auto Attachment : Attachments) {
		if (auto a = Cast<ABarrelAttachment>(Attachment)) {
			BarrelAttachment = a;
			break;
		}
	}
	if(!BarrelAttachment)
		return BarrelExitPoint;
	else 
		return BarrelAttachment->GetBarrelExitLocation();
}



FVector AFirearm::GetHandOffset() const {
	return Hand->GetComponentLocation() - GetActorLocation();
}

void AFirearm::ModifyPivot(FVector& Result, float InitialWeight) const {
	FVector Offset = Hand->GetRelativeLocation() * Hand->LocationWeight;
	float AddedWeight = Hand->LocationWeight;
	if (auto a = Cast<AStockAttachment>(GetAttachment(AStockAttachment::StaticClass()))) {
		AddedWeight += a->Pivot->LocationWeight;
		Result += a->Pivot->GetRelativeLocation() * a->Pivot->LocationWeight;
	}
	Result *= InitialWeight;
	Result += Offset;
	Result /= InitialWeight + AddedWeight;
}

void AFirearm::RegisterImpulse(FVector Impulse) {
	if (auto p = Cast<UFirearmPivot>(GetRootComponent()->GetAttachParent())) {
		p->AddImpulse(Impulse, true);
	}
}

UNiagaraSystem* AFirearm::GetMuzzleFlashSystem() { return DefaultMuzzleFlash; }

void AFirearm::BeginPlay() {
	Super::BeginPlay();

	for (auto InitialAttachment : InitialAttachments) {
		TryAttach(InitialAttachment);
	}
}
