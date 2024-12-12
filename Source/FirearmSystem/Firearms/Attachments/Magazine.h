#pragma once

#include "CoreMinimal.h"
#include "FirearmAttachment.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS()
class FIREARMSYSTEM_API AMagazine : public AFirearmAttachment
{
	GENERATED_BODY()

public:
	AMagazine();

	UPROPERTY(EditDefaultsOnly, Category="Attachment", meta=(UIMin=1))
		int Capacity = 12;
	UPROPERTY(EditDefaultsOnly, Category="Attachment")
		bool bStartFull = true;
	UPROPERTY(EditDefaultsOnly, Category="Attachment")
		TSubclassOf<class ABullet> BulletClass;
	UPROPERTY()
		int CurrentAmmunition = 0;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return CurrentAmmunition<1; }
	
	virtual bool TryUse(class AFirearmBase* Originator) override;
	

protected:
	virtual void BeginPlay() override;
};
