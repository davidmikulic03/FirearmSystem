#pragma once

#include "CoreMinimal.h"
#include "ModularPiece.h"
#include "WeightedBodyContactPoint.h"
#include "Attachments/StockAttachment.h"

#include "Firearm.generated.h"

UENUM()
enum EFiringType : uint8 {
	Automatic,
	SemiAutomatic,
	Manual
};

UCLASS(Abstract)
class FIREARMSYSTEM_API AFirearm : public AModularPiece {
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
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Firearm", meta=(AllowAbstract="false"))
		TArray<TSubclassOf<AFirearmAttachment>> InitialAttachments;
	
	UPROPERTY(EditDefaultsOnly, Category="Firearm", meta=(AllowAbstract="false"))
		TSubclassOf<class ABullet> BulletClass;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(Units="kg", UIMin=0.f))
		float Weight = 2.f;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UWeightedBodyContactPoint* Hand;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		class UWeightedBodyContactPoint* TruePivot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USceneComponent* BarrelExitPoint;

	virtual float GetWeight();
protected:

	void EvaluateTruePivot();

	void RegisterImpulse(FVector Impulse);

	UNiagaraSystem* GetMuzzleFlashSystem();

	
protected:
	virtual void BeginPlay() override;


protected:
	float FireCounter = 0.f;
	int BulletsInChamber = 0;

	
};
