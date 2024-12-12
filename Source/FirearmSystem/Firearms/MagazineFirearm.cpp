
#include "MagazineFirearm.h"

#include "Bullet.h"
#include "FirearmCoreData.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/Magazine.h"
#include "Attachments/UnderBarrelAttachment.h"

AMagazineFirearm::AMagazineFirearm() {

	PrimaryActorTick.bCanEverTick = true;
}

bool AMagazineFirearm::TryFire() {
	if(!Magazine)
		return false;
	if(CanFire() && BarrelAttachment) {
		if(Magazine->TryUse(this)) {
			const FVector Location = BarrelAttachment->FiringPoint->GetComponentLocation();
			const FRotator Rotation = GetActorRotation();
			FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
			SpawnParameters.bNoFail = true;
			if(auto Bullet = GetWorld()->SpawnActor<ABullet>(Magazine->BulletClass,
				Location, Rotation, SpawnParameters)) {
				Bullet->Fire(this, FirearmData->BulletSpeed * GetActorForwardVector());
				FireCounter = 1.f/FirearmData->FireFrequency;
				return true;
			}
		}
	}
	return false;
}

void AMagazineFirearm::BeginPlay() {
	Super::BeginPlay();
	AddMagazine();
}

void AMagazineFirearm::AddMagazine() {
	if(MagazineClass) {
		Magazine = GetWorld()->SpawnActor<AMagazine>();
		Magazine->SetActorLocation(GetActorLocation());
		TryAttach(Magazine);
	}	
}

void AMagazineFirearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryFire();
}

