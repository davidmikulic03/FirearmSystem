// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirearmSystem/Firearms/ModularPiece.h"

#include "FirearmAttachment.generated.h"

UCLASS(Abstract)
class FIREARMSYSTEM_API AFirearmAttachment : public AModularPiece
{
	GENERATED_BODY()

public:
	AFirearmAttachment();

	virtual float GetWeight() { return Weight; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		class UConnector* Connector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(Units="kg", UIMin=0.f))
		float Weight = 0;

	UFUNCTION(BlueprintCallable)
		virtual bool TryUse(class AFirearm* Originator) { return false; }
};
