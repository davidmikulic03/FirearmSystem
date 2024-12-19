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
    float LinearIntegral = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LinearDerivative = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngularProportional = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngularIntegral = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngularDerivative = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IntegralTermFalloffTime = 0.f;

    static FRecoilResistParams SoftNormalizedSum(FRecoilResistParams a, FRecoilResistParams b) {
        FRecoilResistParams Result;

        if(b.RecoilRandomness >= 0) Result.RecoilRandomness = a.RecoilRandomness + b.RecoilRandomness;
        else Result.RecoilRandomness = a.RecoilRandomness * FMath::Exp(b.RecoilRandomness / a.RecoilRandomness);

        if(b.LinearProportional >= 0) Result.LinearProportional = a.LinearProportional + b.LinearProportional;
        else Result.LinearProportional = a.LinearProportional * FMath::Exp(b.LinearProportional / a.LinearProportional);
        
        if(b.LinearDerivative >= 0) Result.LinearDerivative = a.LinearDerivative + b.LinearDerivative;
        else Result.LinearDerivative = a.LinearDerivative * FMath::Exp(b.LinearDerivative / a.LinearDerivative);

        if(b.AngularProportional >= 0) Result.AngularProportional = a.AngularProportional + b.AngularProportional;
        else Result.AngularProportional = a.AngularProportional * FMath::Exp(b.AngularProportional / a.AngularProportional);

        if(b.AngularIntegral >= 0) Result.AngularIntegral = a.AngularIntegral + b.AngularIntegral;
        else Result.AngularIntegral = a.AngularIntegral * FMath::Exp(b.AngularIntegral / a.AngularIntegral);

        if(b.AngularDerivative >= 0) Result.AngularDerivative = a.AngularDerivative + b.AngularDerivative;
        else Result.AngularDerivative = a.AngularDerivative * FMath::Exp(b.AngularDerivative / a.AngularDerivative);

        if(b.IntegralTermFalloffTime >= 0) Result.IntegralTermFalloffTime = a.IntegralTermFalloffTime + b.IntegralTermFalloffTime;
        else Result.IntegralTermFalloffTime = a.IntegralTermFalloffTime * FMath::Exp(b.IntegralTermFalloffTime / a.IntegralTermFalloffTime);
        
        return Result;
    }
    
    FRecoilResistParams operator+=(const FRecoilResistParams& Other) {
        RecoilRandomness += Other.RecoilRandomness;
        LinearProportional += Other.LinearProportional;
        LinearIntegral += Other.LinearIntegral;
        LinearDerivative += Other.LinearDerivative;
        AngularProportional += Other.AngularProportional;
        AngularIntegral += Other.AngularIntegral;
        AngularDerivative += Other.AngularDerivative;
        IntegralTermFalloffTime += Other.IntegralTermFalloffTime;
        return *this;
    }
};