#include "FirearmPivot.h"

#include "Firearm.h"
#include "SceneRenderTargetParameters.h"
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
	FVector Offset = GetComponentLocation() - Firearm->Hand->GetComponentLocation();
	Firearm->AddActorWorldOffset(Offset);
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
		FVector RotationVector = ResistParams.RecoilRandomness * UKismetMathLibrary::RandomUnitVector();
		FQuat Rotation = FQuat::MakeFromRotationVector(RotationVector);
		Impulse = Rotation * Impulse;
	}
	FVector Offset = Firearm->GetActorLocation() - Firearm->TruePivot->GetComponentLocation();
	FVector AngularImpulse = Offset.Cross(Impulse) / 100.f;
	FVector LinearImpulse = Impulse;

	FTransform ToLocal = GetComponentTransform().Inverse();
	LinearVelocity+=LinearImpulse;
	AngularVelocity+=ToLocal.TransformVectorNoScale(AngularImpulse);
}

void UFirearmPivot::UpdateState(float DeltaSeconds) {
	// if (DeltaSeconds * AngularVelocity.Length() > 20)

	FTransform ToWorld = GetComponentTransform();
	FQuat DeltaRotation = FQuat::MakeFromRotationVector(ToWorld.TransformVectorNoScale(AngularVelocity * DeltaSeconds));
	FVector Offset = Firearm->TruePivot->GetComponentLocation() - Firearm->Root->GetComponentLocation();
	Firearm->Root->AddWorldOffset(Offset, false, nullptr, ETeleportType::TeleportPhysics);
	Firearm->Root->AddWorldRotation(DeltaRotation, false, nullptr, ETeleportType::TeleportPhysics);
	Firearm->Root->AddWorldOffset(-Offset, false, nullptr, ETeleportType::TeleportPhysics);
	Firearm->Root->AddWorldOffset(LinearVelocity * DeltaSeconds, false, nullptr, ETeleportType::TeleportPhysics);
	
}

void UFirearmPivot::Resist(float DeltaSeconds) {
	ResistLinear(DeltaSeconds);
	ResistAngular(DeltaSeconds);
}

void UFirearmPivot::ResistLinear(float DeltaSeconds) {
	FVector DeltaPosition = GetComponentLocation() - Firearm->Hand->GetComponentLocation();
	FVector DeltaVelocity = -LinearVelocity;
	FVector Acceleration = ResistParams.LinearProportional * DeltaPosition + ResistParams.LinearDerivative * DeltaVelocity;
	LinearVelocity += Acceleration * DeltaSeconds;
}

void UFirearmPivot::ResistAngular(float DeltaSeconds) {
	FTransform ToWorld = GetComponentTransform();
	FQuat DeltaRotation = GetComponentRotation().Quaternion().Inverse() * Firearm->GetActorRotation().Quaternion();
	FVector AsRotationVector = DeltaRotation.ToRotationVector();
	FVector Torque = -Firearm->GetWeight() * (ResistParams.AngularProportional * AsRotationVector + ResistParams.AngularDerivative * AngularVelocity);
	AngularVelocity += Torque * DeltaSeconds;
	// FVector Torque = -Firearm->GetWeight() * (AngularProportional * AsRotationVector + AngularDerivative * ToWorld.TransformVectorNoScale(AngularVelocity));
	// AngularVelocity += ToWorld.InverseTransformVectorNoScale(Torque * DeltaSeconds);
}


