
#include "MagazineFirearm.h"

#include "Bullet.h"
#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/Magazine.h"
#include "Attachments/UnderBarrelAttachment.h"
#include "Kismet/KismetMathLibrary.h"

AMagazineFirearm::AMagazineFirearm() {

	PrimaryActorTick.bCanEverTick = true;
}

bool AMagazineFirearm::TryFire() {
	if(!Magazine)
		return false;
	if(CanFire() && BarrelAttachment && Magazine->BulletClass && Magazine->TryUse(this)) {
		const FVector Location = BarrelAttachment->BarrelExitPoint->GetComponentLocation();
		const FRotator Rotation = GetActorRotation();
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.bNoFail = true;
		SpawnParameters.Owner = this;
		if(auto Bullet = GetWorld()->SpawnActor<ABullet>(Magazine->BulletClass,
			Location, Rotation, SpawnParameters))
		{
			float AverageErrorAngleInRadians = BarrelAttachment->AccuracyModifier * FMath::DegreesToRadians(FirearmData->Accuracy / 60);
			FVector Direction = GetActorForwardVector();
			FQuat RandomRotation = FQuat::MakeFromRotationVector(AverageErrorAngleInRadians * UKismetMathLibrary::RandomUnitVector());
			Direction = RandomRotation * Direction;
			Bullet->Fire(this, FirearmData->BulletSpeed * Direction);
			FireCounter = 1.f/FirearmData->FireFrequency;
			return true;
		}
	}
	else if(Magazine->IsEmpty())
		AddMagazine();
	return false;
}

void AMagazineFirearm::BeginPlay() {
	Super::BeginPlay();
	AddMagazine();
}

void AMagazineFirearm::AddMagazine() {
	if(MagazineClass) {
		if(Magazine) {
			Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Magazine->Mesh->SetSimulatePhysics(true);
		}
		Magazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass);
		TryAttach(Magazine);
	}	
}

void AMagazineFirearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryFire();
}

