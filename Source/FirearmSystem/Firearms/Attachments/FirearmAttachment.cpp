// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmAttachment.h"


// Sets default values
AFirearmAttachment::AFirearmAttachment() {

	AttachmentPoint = CreateDefaultSubobject<USceneComponent>("Attachment Point");
	RootComponent = AttachmentPoint;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(AttachmentPoint);
	
	PrimaryActorTick.bCanEverTick = false;
}

