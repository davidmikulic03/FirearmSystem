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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
		float Health = 100.0f;
};
