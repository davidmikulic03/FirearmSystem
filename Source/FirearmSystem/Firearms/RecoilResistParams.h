#pragma once

#include "CoreMinimal.h"
#include "RecoilResistParams.generated.h"

USTRUCT(BlueprintType)
struct FRecoilResistParams {
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Units="rad"))
    float RecoilRandomness = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LinearProportional = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LinearDerivative = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngularProportional = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngularDerivative = 0.f;
    
    FRecoilResistParams operator+=(const FRecoilResistParams& Other) {
        
        RecoilRandomness += Other.RecoilRandomness;
        LinearProportional += Other.LinearProportional;
        LinearDerivative += Other.LinearDerivative;
        AngularProportional += Other.AngularProportional;
        AngularDerivative += Other.AngularDerivative;
        return *this;
    }
};