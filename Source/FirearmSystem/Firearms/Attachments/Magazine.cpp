#include "Magazine.h"

#include "FirearmSystem/Firearms/FirearmBase.h"

AMagazine::AMagazine() {

	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	PrimaryActorTick.bCanEverTick = false;
}

bool AMagazine::TryUse(AFirearmBase* Originator) {
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


