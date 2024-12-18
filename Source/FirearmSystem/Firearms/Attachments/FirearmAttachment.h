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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		class UConnector* Connector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		float Health = 100.0f;

	UFUNCTION(BlueprintCallable)
		virtual bool TryUse(class AFirearm* Originator) { return false; }
};
