// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmAttachment.h"

#include "Connector.h"
#include "Components/ArrowComponent.h"


// Sets default values
AFirearmAttachment::AFirearmAttachment() {

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Connector = CreateDefaultSubobject<UConnector>("Connector");
	Connector->SetupAttachment(Mesh);
	
	PrimaryActorTick.bCanEverTick = false;
}

