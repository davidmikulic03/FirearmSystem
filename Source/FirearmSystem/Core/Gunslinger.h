// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Gunslinger.generated.h"


class UInputAction;
struct FInputActionValue;

UCLASS(Abstract)
class FIREARMSYSTEM_API AGunslinger : public ACharacter
{
	GENERATED_BODY()

public:
	AGunslinger();
	UFUNCTION()
		void BeginMoving(const FInputActionValue& Value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginMoving();
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void FinishMoving(const FInputActionValue& Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnFinishMoving();

	UFUNCTION()
	void BeginPanning(const FInputActionValue& Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginPanning();
	
	UFUNCTION()
	void Pan(const FInputActionValue& Value);

	UFUNCTION()
	void FinishPanning(const FInputActionValue& Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnFinishPanning();

	UFUNCTION()
	void StartCrouch(const FInputActionValue& Value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& Value);
	
	UFUNCTION()
	void SingleFire(const FInputActionValue& Value);
	UFUNCTION()
	void StartFiring(const FInputActionValue& Value);
	UFUNCTION()
	void StopFiring(const FInputActionValue& Value);
	bool bIsFiring = false;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enhanced Input", meta=(AllowPrivateAccess="true"))
		class UInputMappingContext* CharacterContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* PanAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* ManualFireAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* AutomaticFireAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
		UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
		class UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Firearm", meta=(AllowPrivateAccess="true"))
		class UFirearmPivot* FirearmPivot;
};
