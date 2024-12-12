// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmAttachment.h"


// Sets default values
AFirearmAttachment::AFirearmAttachment() {

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	AttachmentPoint = CreateDefaultSubobject<USceneComponent>("Attachment Point");
	AttachmentPoint->SetupAttachment(Mesh);
	
	PrimaryActorTick.bCanEverTick = false;
}

