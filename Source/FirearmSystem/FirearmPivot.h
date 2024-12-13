// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Firearms/FirearmBase.h"
#include "FirearmPivot.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIREARMSYSTEM_API UFirearmPivot : public USceneComponent
{
	GENERATED_BODY()

public:
	UFirearmPivot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LinearProportional = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LinearDerivative = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AngularProportional = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AngularDerivative = 5.f;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool TryFire();
	UFUNCTION(BlueprintCallable)
		bool Equip(class AFirearmBase* InFirearm);

	void AddImpulse(FVector Impulse);
	
	UPROPERTY()
		USceneComponent* Target;
	UPROPERTY(EditInstanceOnly)
		class AFirearmBase* Firearm;
	
protected:
	void UpdateState(float DeltaSeconds);
	void Resist(float DeltaSeconds);
	void ResistLinear(float DeltaSeconds);
	void ResistAngular(float DeltaSeconds);
	
	FVector LinearVelocity = FVector::ZeroVector;
	FVector AngularVelocity = FVector::ZeroVector;
	
};
