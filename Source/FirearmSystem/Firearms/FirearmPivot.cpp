#include "FirearmPivot.h"

#include "Firearm.h"
#include "SceneRenderTargetParameters.h"
#include "FirearmSystem/Core/Gunslinger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SceneQueries/SceneSnappingManager.h"

UFirearmPivot::UFirearmPivot() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UFirearmPivot::BeginPlay() {
	Super::BeginPlay();
	// Target = Cast<USceneComponent>(GetOwner()->AddComponentByClass(USceneComponent::StaticClass(), false, GetOwner()->GetTransform(), true));
}

void UFirearmPivot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	DeltaTime = FMath::Clamp(DeltaTime, 0.f, 1.f/30);
	// ClampVelocities();
	UpdateState(DeltaTime);
	Resist(DeltaTime);
	// IntegralAngularError *= exp(-DeltaTime/IntegralTermFalloffSpeed);
}

bool UFirearmPivot::TryFire() {
	return Firearm ? Firearm->TryFire() : false;
}

bool UFirearmPivot::Equip(class AFirearm* InFirearm) {
	if (!InFirearm)
		return false;
	Firearm = InFirearm;
	FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepRelativeTransform;
	Rules.bWeldSimulatedBodies = true;
	Firearm->Root->AttachToComponent(this, Rules);
	Firearm->SetActorRotation(GetComponentRotation());
	FVector HandOffset = Firearm->Hand->GetComponentLocation() - Firearm->GetActorLocation();
	Firearm->SetActorRelativeLocation(Firearm->Hand->GetRelativeLocation());
	TArray<AActor*> Children;
	Firearm->GetAttachedActors(Children);
	Children.Add(Firearm);
	for (auto Child : Children) {
		Child->SetActorEnableCollision(false);
	}
	return true;
}

void UFirearmPivot::AddImpulse(FVector Impulse, bool bRandomize) {
	if (bRandomize) {
		float GaussianModifier = sqrt(-2 * log(FMath::FRand()));
		FVector RotationVector = GaussianModifier * ResistParams.RecoilRandomness * UKismetMathLibrary::RandomUnitVector();
		FQuat Rotation = FQuat::MakeFromRotationVector(RotationVector);
		Impulse = Rotation * Impulse;
	}
	FVector Offset = Firearm->GetActorLocation() - (GetComponentLocation() + GetComponentTransform().TransformVectorNoScale(CenterOfMass));
	FVector AngularImpulse = Offset.Cross(Impulse) / 100.f;
	FVector LinearImpulse = Impulse;

	FTransform ToLocal = GetComponentTransform().Inverse();
	LinearVelocity+=LinearImpulse;
	AngularVelocity+=ToLocal.TransformVectorNoScale(AngularImpulse);
}

void UFirearmPivot::UpdateState(float DeltaSeconds) {
	
	FTransform ToWorld = GetComponentTransform();
	FQuat DeltaRotation = FQuat::MakeFromRotationVector(ToWorld.TransformVectorNoScale(AngularVelocity * DeltaSeconds));
	// FVector Initial = Firearm->Hand->GetComponentLocation();
	FVector WeightedPivot = FVector::ZeroVector;
	float PivotWeight = 0.f;
	Firearm->GetWeightedPivotLocation(WeightedPivot, PivotWeight);
	FVector PivotLocation = WeightedPivot / PivotWeight;
	FVector ToPivot = PivotLocation - GetComponentLocation();
	// FVector WorldCOM = ToWorld.TransformVectorNoScale(CenterOfMass);
	Firearm->Root->AddWorldOffset(-ToPivot, false, nullptr, ETeleportType::TeleportPhysics);
	Firearm->Root->AddWorldRotation(DeltaRotation, false, nullptr, ETeleportType::TeleportPhysics);
	Firearm->Root->AddWorldOffset(ToPivot, false, nullptr, ETeleportType::TeleportPhysics);
	// Firearm->Root->AddWorldOffset(-WorldCOM, false, nullptr, ETeleportType::TeleportPhysics);
	
	Firearm->Root->AddWorldOffset(LinearVelocity * DeltaSeconds, false, nullptr, ETeleportType::TeleportPhysics);
	
}

void UFirearmPivot::Resist(float DeltaSeconds) {
	ResistLinear(DeltaSeconds);
	ResistAngular(DeltaSeconds);
}

void UFirearmPivot::ResistLinear(float DeltaSeconds) {
	FVector WeightedPivot = FVector::ZeroVector;
	float PivotWeight = 0.f;
	Firearm->GetWeightedPivotLocation(WeightedPivot, PivotWeight);
	FVector PivotLocation = WeightedPivot / PivotWeight;
	DrawDebugBox(GetWorld(), PivotLocation, FVector::OneVector * 5.f, FColor::Green);
	FVector DeltaPosition = GetComponentLocation() - PivotLocation;
	FVector DeltaVelocity = -LinearVelocity;
	FVector Acceleration = (ResistParams.LinearProportional * DeltaPosition + ResistParams.LinearDerivative * DeltaVelocity) / Firearm->GetWeight();
	LinearVelocity += Acceleration * DeltaSeconds;
}

void UFirearmPivot::ResistAngular(float DeltaSeconds) {
	FTransform ToWorld = GetComponentTransform();
	FQuat PivotCorrection = FQuat::FindBetweenNormals(GetForwardVector(), GunSlinger->GetGunTargetDirection());
	FQuat DeltaRotation = (PivotCorrection * GetComponentRotation().Quaternion()).Inverse() * Firearm->GetActorRotation().Quaternion();
	
	FVector AsRotationVector = DeltaRotation.ToRotationVector();
	if (AsRotationVector.X > PI) AsRotationVector.X = PI - AsRotationVector.X;
	if (AsRotationVector.Y > PI) AsRotationVector.Y = PI - AsRotationVector.Y;
	if (AsRotationVector.Z > PI) AsRotationVector.Z = PI - AsRotationVector.Z;
	
	if (!bIsIdle) {
		IntegralAngularError *= exp(-DeltaSeconds/ResistParams.IntegralTermFalloffTime);
		IntegralAngularError += AsRotationVector * DeltaSeconds;
	}
	
	FVector P = -ResistParams.AngularProportional * AsRotationVector;
	FVector I = -ResistParams.AngularIntegral * IntegralAngularError;
	FVector D = -ResistParams.AngularDerivative * AngularVelocity;
	float Weight = Firearm->GetWeight();
	FVector Torque = (P+I+D) / Weight;
	AngularVelocity += /*ToWorld.InverseTransformVectorNoScale*/(Torque) * DeltaSeconds;
	// FVector Torque = -Firearm->GetWeight() * (AngularProportional * AsRotationVector + AngularDerivative * ToWorld.TransformVectorNoScale(AngularVelocity));
	// AngularVelocity += ToWorld.InverseTransformVectorNoScale(Torque * DeltaSeconds);
}


