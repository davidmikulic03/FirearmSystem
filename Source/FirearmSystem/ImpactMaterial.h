#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ImpactMaterial.generated.h"

UENUM()
enum EImpactMaterial : uint8 {
	PlyWood,
	Oak,
	Water,
	Steel,
	Rock,
	Lead
};
namespace Utility {
	static float GetDensityOf(EImpactMaterial Material) {
		switch (Material) {
		case PlyWood:
			return 0.35f;
		case Oak:
			return 0.8f;
		case Water:
			return 1.f;
		case Steel:
			return 7.85f;
		case Rock:
			return 2.75f;
		case Lead:
			return 11.35f;
		default:
			return 0.f;
		}
	}
}