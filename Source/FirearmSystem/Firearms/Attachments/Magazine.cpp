#include "Magazine.h"

#include "FirearmSystem/Firearms/Firearm.h"

AMagazine::AMagazine() {

	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	PrimaryActorTick.bCanEverTick = false;
}

bool AMagazine::TryUse(AFirearm* Originator) {
	if(!IsEmpty()) {
		CurrentAmmunition--;
		return true;
	}
	return false;
}

void AMagazine::BeginPlay() {
	if(bStartFull)
		CurrentAmmunition = Capacity;
}


