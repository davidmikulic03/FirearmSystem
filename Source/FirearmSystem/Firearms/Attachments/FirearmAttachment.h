// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirearmAttachment.generated.h"

UCLASS()
class FIREARMSYSTEM_API AFirearmAttachment : public AActor
{
	GENERATED_BODY()

public:
	AFirearmAttachment();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		USceneComponent* AttachmentPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(Units="kg", UIMin=0.f))
		float Weight = 0;

	UFUNCTION(BlueprintCallable)
		virtual bool TryUse(class AFirearmBase* Originator) { return false; }
};
