#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attachments/Connector.h"

#include "ModularPiece.generated.h"

class AFirearmAttachment;

UCLASS()
class FIREARMSYSTEM_API AModularPiece : public AActor {
	GENERATED_BODY()

public:
	AModularPiece();

public:
	virtual void BeginPlay() override;

	class AFirearmAttachment* GetAttachment(UClass* InClass) const;

	template<typename T = AFirearmAttachment>
	T* GetAttachment() const;
	
	UConnector* GetConnector(UConnectorType* Type) const;

	bool TryAttach(AFirearmAttachment* InActor);
	AFirearmAttachment* TryAttach(TSubclassOf<class AFirearmAttachment> InClass);
	
	bool TryDetach(AFirearmAttachment* InActor);

	virtual float GetWeight();

	struct FRecoilResistParams GetResistParams() const;
	void GetWeightedPivotLocation(FVector& OutLocation, float& OutWeight) const;

	UPROPERTY(VisibleInstanceOnly)
		TArray<class AFirearmAttachment*> Attachments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment", meta=(Units="kg", UIMin=0.f))
		float Weight = 0;
};


