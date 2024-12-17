#include "ModularPiece.h"
#include "Attachments/Connector.h"
#include "Attachments/FirearmAttachment.h"

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
			FVector Offset = c->GetRelativeLocation() - InActor->Connector->GetRelativeLocation();
			InActor->SetActorLocation(c->GetComponentLocation() + Offset);
			InActor->SetActorRotation(c->GetComponentRotation());
			Attachments.Add(InActor);
			return true;
		}
	}
	return false;
}

bool AModularPiece::TryAttach(TSubclassOf<class AFirearmAttachment> InClass) {
	if (auto a = GetWorld()->SpawnActor<AFirearmAttachment>(InClass))
		return TryAttach(a);
	return false;
}

bool AModularPiece::TryDetach(AFirearmAttachment* InActor) {
	if (InActor) {
		InActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Attachments.Remove(InActor);
		return true;
	}
	return false;
}
