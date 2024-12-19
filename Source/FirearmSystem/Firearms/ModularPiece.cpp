#include "ModularPiece.h"

#include "RecoilResistParams.h"
#include "Attachments/Connector.h"
#include "Attachments/FirearmAttachment.h"
#include "WeightedContactPoint.h"


AModularPiece::AModularPiece() {
	PrimaryActorTick.bCanEverTick = false;
}

void AModularPiece::BeginPlay() {
	Super::BeginPlay();
	
}

UConnector* AModularPiece::GetConnector(class UConnectorType* Type) const {
	TArray<UConnector*> Components;
	GetComponents<UConnector>(Components);
	for (auto Connector : Components) {
		if (Connector && Connector->Self && Connector->Self->AvailableConnection && Connector->Self->AvailableConnection == Type)
			return Connector;
	}
	return nullptr;
}

AFirearmAttachment* AModularPiece::GetAttachment(UClass* InClass) const {
	for (auto Attachment : Attachments) {
		if (Attachment->IsA(InClass)) {
			return Attachment;
		}
	}
	return nullptr;
}

template <typename T>
T* AModularPiece::GetAttachment() const {
	return Cast<T>(GetAttachment(T::StaticClass()));
}

bool AModularPiece::TryAttach(AFirearmAttachment* InActor) {
	if(InActor) {
		if (auto c = GetConnector(InActor->Connector->Self)) {
			FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepRelativeTransform;
			Rules.bWeldSimulatedBodies = true;
			InActor->AttachToComponent(c, Rules);
			InActor->SetActorRotation(c->GetComponentRotation());
			FVector Offset = c->GetComponentLocation() - InActor->Connector->GetComponentLocation();
			InActor->AddActorWorldOffset(Offset);
			Attachments.Add(InActor);
			return true;
		}
	}
	return false;
}

AFirearmAttachment* AModularPiece::TryAttach(TSubclassOf<class AFirearmAttachment> InClass) {
	if (auto a = GetWorld()->SpawnActor<AFirearmAttachment>(InClass))
		if (TryAttach(a)) return a;
	return nullptr;
}

bool AModularPiece::TryDetach(AFirearmAttachment* InActor) {
	if (InActor) {
		InActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Attachments.Remove(InActor);
		return true;
	}
	return false;
}

float AModularPiece::GetWeight() {
	float Result = Weight;
	for (auto Attachment : Attachments) {
		if (Attachment) 
			Result += Attachment->GetWeight();
	}
	return Result;
}

FRecoilResistParams AModularPiece::GetResistParams() const {
	FRecoilResistParams Result;
	TArray<UWeightedContactPoint*> ContactPoints;
	GetComponents<UWeightedContactPoint*>(ContactPoints);
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto ContactPoint : ContactPoints) {
		Result += ContactPoint->ResistParams;
	}
	for (auto Child : AttachedActors) {
		if (auto f = Cast<AFirearmAttachment>(Child)) {
			Result += f->GetResistParams();
		}
	}
	return Result;
}

void AModularPiece::GetWeightedPivotLocation(FVector& OutLocation, float& OutWeight) const {
	TArray<UWeightedContactPoint*> ContactPoints;
	GetComponents<UWeightedContactPoint*>(ContactPoints);
	for (auto ContactPoint : ContactPoints) {
		OutLocation += ContactPoint->GetComponentLocation() * ContactPoint->LocationWeight;
		OutWeight += ContactPoint->LocationWeight;
	}
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto Child : AttachedActors) {
		if (auto f = Cast<AFirearmAttachment>(Child))
			f->GetWeightedPivotLocation(OutLocation, OutWeight);
	}
}
