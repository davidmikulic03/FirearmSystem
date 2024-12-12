#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FirearmBase.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AFirearmBase : public AActor {
	GENERATED_BODY()

public:
	AFirearmBase();

	UFUNCTION(BlueprintCallable)
		virtual bool TryFire();
	virtual bool CanFire();
	virtual void Tick(float DeltaSeconds) override;

	void RegisterHit(TWeakObjectPtr<UPrimitiveComponent> Component);

	FVector GetBarrelExitLocation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFirearmCoreData* FirearmData;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class ABarrelAttachment* BarrelAttachment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* BarrelExitPoint;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class AStockAttachment* StockAttachment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* StockAttachmentPoint;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class AOpticsAttachment* OpticsAttachment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* OpticsAttachmentPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AUnderBarrelAttachment* UnderBarrelAttachment;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* UnderBarrelAttachmentPoint;

	UPROPERTY(EditInstanceOnly)
		TSubclassOf<ABarrelAttachment> InitialBarrelAttachmentClass;
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AStockAttachment> InitialStockAttachmentClass;
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AOpticsAttachment> InitialOpticsAttachmentClass;
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AUnderBarrelAttachment> InitialUnderBarrelAttachmentClass;
	
	UFUNCTION(BlueprintCallable)
		void AttachBarrel(ABarrelAttachment* InBarrel, ABarrelAttachment*& OutBarrel);
	UFUNCTION(BlueprintCallable)
		void AttachStock(AStockAttachment* InStock, AStockAttachment*& OutStock);
	UFUNCTION(BlueprintCallable)
		void AttachOptics(AOpticsAttachment* InOptics, AOpticsAttachment*& OutOptics);
	UFUNCTION(BlueprintCallable)
		void AttachUnderBarrel(AUnderBarrelAttachment* InUnderBarrel, AUnderBarrelAttachment*& OutUnderBarrel);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasBarrelAttachment() const { return BarrelAttachment!=nullptr; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasStockAttachment() const { return StockAttachment!=nullptr; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasOpticsAttachment() const { return OpticsAttachment!=nullptr; }

protected:
	bool TryAttach(AActor* InActor);
	bool TryDetach(AActor* InActor);
protected:
	virtual void BeginPlay() override;

protected:
	float FireCounter = 0.f;
	int BulletsInChamber = 0;
};


