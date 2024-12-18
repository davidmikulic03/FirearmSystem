// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecoilResistParams.h"
#include "Components/SceneComponent.h"
#include "Firearm.h"
#include "FirearmPivot.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIREARMSYSTEM_API UFirearmPivot : public USceneComponent
{
	GENERATED_BODY()

public:
	UFirearmPivot();
	FRecoilResistParams ResistParams;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool TryFire();
	UFUNCTION(BlueprintCallable)
		bool Equip(class AFirearm* InFirearm);

	void AddImpulse(FVector Impulse, bool bRandomize = false);
	
	void SetOwner(class AGunslinger* InOwner) { Gunslinger = InOwner; }
	
	UPROPERTY()
		USceneComponent* Target;
	UPROPERTY(EditInstanceOnly)
		class AFirearm* Firearm;
	
	class AGunslinger* Gunslinger;
	
	FVector CenterOfMass = FVector::ZeroVector;
protected:
	void UpdateState(float DeltaSeconds);
	void Resist(float DeltaSeconds);
	void ResistLinear(float DeltaSeconds);
	void ResistAngular(float DeltaSeconds);
	
	FVector LinearVelocity = FVector::ZeroVector;
	FVector AngularVelocity = FVector::ZeroVector;
};
