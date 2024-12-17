#pragma once

#include "CoreMinimal.h"
#include "WeightedBodyContactPoint.h"
#include "Attachments/StockAttachment.h"
#include "GameFramework/Actor.h"


#include "Firearm.generated.h"

UENUM()
enum EFiringType : uint8 {
	Automatic,
	SemiAutomatic,
	Manual
};

UCLASS(Abstract)
class FIREARMSYSTEM_API AFirearm : public AActor {
	GENERATED_BODY()

public:
	AFirearm();

	UFUNCTION(BlueprintCallable)
		virtual bool TryFire();
	virtual bool CanFire();
	virtual void Tick(float DeltaSeconds) override;

	void RegisterHit(FHitResult Hit);

	USceneComponent* GetBarrelExit();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm")
		FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm")
		TEnumAsByte<EFiringType> FiringType = Manual;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(EditCondition="FiringType!=EFiringType::Manual", EditConditionHides))
		bool bNeedsInitialCocking = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(Units="m/s"))
		float BulletSpeed = 400;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(UIMin=0.f))
		float Accuracy = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(UIMin=0.f))
		float FireFrequency = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(UIMin=1))
		int ChamberCapacity = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(AllowAbstract="false"))
		TSubclassOf<class ABarrelAttachment> DefaultBarrelClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(AllowAbstract="false"))
		TSubclassOf<class AStockAttachment> DefaultStockClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(AllowAbstract="false"))
		 class UNiagaraSystem* DefaultMuzzleFlash;
	
	UPROPERTY(EditDefaultsOnly, Category="Firearm", meta=(AllowAbstract="false"))
		TSubclassOf<class ABullet> BulletClass;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(Units="kg", UIMin=0.f))
		float Weight = 2.f;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UWeightedBodyContactPoint* Hand;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class UWeightedBodyContactPoint* TruePivot;
	
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

	UPROPERTY(EditInstanceOnly, Category="Firearm")
		TSubclassOf<ABarrelAttachment> InitialBarrelAttachmentClass;
	UPROPERTY(EditInstanceOnly, Category="Firearm")
		TSubclassOf<AStockAttachment> InitialStockAttachmentClass;
	UPROPERTY(EditInstanceOnly, Category="Firearm")
		TSubclassOf<AOpticsAttachment> InitialOpticsAttachmentClass;
	UPROPERTY(EditInstanceOnly, Category="Firearm")
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

	virtual float GetWeight();
protected:
	bool TryAttach(AActor* InActor);
	bool TryDetach(AActor* InActor);

	void EvaluateTruePivot();

	void RegisterImpulse(FVector Impulse);

	UNiagaraSystem* GetMuzzleFlashSystem();
protected:
	virtual void BeginPlay() override;


protected:
	float FireCounter = 0.f;
	int BulletsInChamber = 0;

	
};
