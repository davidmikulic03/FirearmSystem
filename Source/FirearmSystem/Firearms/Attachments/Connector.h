#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Connector.generated.h"


UCLASS(Blueprintable, BlueprintType)
class FIREARMSYSTEM_API UConnectorType : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UConnectorType* AvailableConnection;
};

UCLASS(meta=(BlueprintSpawnableComponent))
class FIREARMSYSTEM_API UConnector : public USceneComponent {
	GENERATED_BODY()
public:
	UConnector() { PrimaryComponentTick.bCanEverTick = false; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UConnectorType* Self;
		
	UPROPERTY()
		UConnector* Connection = nullptr;

	bool AttachToSelf(UConnector* InConnection) {
		if (!Self || InConnection->Self || Self != InConnection->Self)
			return false;
		Connection = InConnection;
		InConnection->Connection = this;
		auto TransformRules = FAttachmentTransformRules::KeepRelativeTransform;
		TransformRules.bWeldSimulatedBodies = true;
		Connection->AttachToComponent(this, TransformRules);
		return true;
	}
	bool AttachSelfToOther(UConnector* InConnection) {
		if (!Self || InConnection->Self || Self != InConnection->Self)
			return false;
		Connection = InConnection;
		InConnection->Connection = this;
		auto TransformRules = FAttachmentTransformRules::KeepRelativeTransform;
		TransformRules.bWeldSimulatedBodies = true;
		AttachToComponent(Connection, TransformRules);
		return true;
	}
};
