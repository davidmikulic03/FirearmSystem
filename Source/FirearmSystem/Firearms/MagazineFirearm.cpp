
#include "MagazineFirearm.h"

#include "Bullet.h"
#include "FirearmCoreData.h"
#include "ParticleHelper.h"
#include "Attachments/BarrelAttachment.h"
#include "Attachments/Magazine.h"
#include "Attachments/UnderBarrelAttachment.h"
#include "Kismet/KismetMathLibrary.h"

AMagazineFirearm::AMagazineFirearm() {

	PrimaryActorTick.bCanEverTick = true;
}

bool AMagazineFirearm::TryFire() {
	bool bResult = Super::TryFire();
	
	if (FirearmData->FiringType!=Manual && Magazine) {
		if (BulletsInChamber < FirearmData->ChamberCapacity && Magazine->TryUse(this))
			BulletsInChamber++;
		else if(Magazine->IsEmpty())
			ReplaceMagazine();
	}
	else if (!Magazine)
		ReplaceMagazine();
	
	return bResult;
}

void AMagazineFirearm::BeginPlay() {
	Super::BeginPlay();
}

void AMagazineFirearm::ReplaceMagazine() {
	if(MagazineClass && Cast<AMagazine>(MagazineClass->ClassDefaultObject)->BulletClass == FirearmData->BulletClass) {
		if(Magazine) {
			Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Magazine->Mesh->SetSimulatePhysics(true);
		}
		Magazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass);
		TryAttach(Magazine);
	}	
}

float AMagazineFirearm::GetWeight()
{
	float Result = Super::GetWeight();
	if (Magazine)
		Result += Magazine->GetWeight();
	return Result;
}

void AMagazineFirearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryFire();
}

