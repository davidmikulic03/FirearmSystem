#include "Firearm.h"

#include "Bullet.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/StockAttachment.h"
#include "FirearmPivot.h"
#include "WeightedBodyContactPoint.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Attachments/Connector.h"
#include "Kismet/KismetMathLibrary.h"

AFirearm::AFirearm() {
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Firearm");
	RootComponent = Root;
	Hand = CreateDefaultSubobject<UWeightedBodyContactPoint>("Pivot");
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

void AFirearm::EvaluateTruePivot() {
	if (!TruePivot) {
		TruePivot = Cast<UWeightedBodyContactPoint>(AddComponentByClass(UWeightedBodyContactPoint::StaticClass(), false, GetTransform(), false));
	}
	float WeightSum = Hand->LocationWeight;
	TruePivot->SetRelativeLocation(Hand->GetRelativeLocation() * Hand->LocationWeight);
	TruePivot->ResistParams = Hand->ResistParams;
	if (auto c = Cast<AStockAttachment>(GetAttachment(AStockAttachment::StaticClass()))) {
		WeightSum += c->Pivot->LocationWeight;
		TruePivot->ResistParams += c->Pivot->ResistParams;
		TruePivot->AddRelativeLocation(c->Pivot->GetRelativeLocation() * c->Pivot->LocationWeight);
	}
	TruePivot->SetRelativeLocation(TruePivot->GetRelativeLocation() / WeightSum);
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
	
	EvaluateTruePivot();
}

float AFirearm::GetWeight() {
	float Result = Weight;
	for (auto Attachment : Attachments) {
		if (Attachment) 
			Weight += Attachment->GetWeight();
	}
	return Result;
}
