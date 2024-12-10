#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Firearm.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AFirearm : public AActor {
	GENERATED_BODY()

public:
	AFirearm();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Root;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFirearmCoreData* FirearmData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UChildActorComponent* BarrelAttachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UChildActorComponent* StockAttachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UChildActorComponent* OpticAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentAmmunition;
};
