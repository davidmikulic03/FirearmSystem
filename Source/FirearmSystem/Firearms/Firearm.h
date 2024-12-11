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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABarrelAttachment* BarrelAttachment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AStockAttachment* StockAttachment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AOpticAttachment* OpticAttachment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentAmmunition;
	
	UFUNCTION(BlueprintCallable)
		void AttachBarrel(ABarrelAttachment* InBarrel, ABarrelAttachment*& OutBarrel);
	UFUNCTION(BlueprintCallable)
		void AttachStock(AStockAttachment* InStock, AStockAttachment*& OutStock);
	UFUNCTION(BlueprintCallable)
		void AttachOptics(AOpticAttachment* InOptics, AOpticAttachment*& OutOptics);
private:
	bool TryAttach(AActor* InActor);
	bool TryDetach(AActor* InActor);
};


