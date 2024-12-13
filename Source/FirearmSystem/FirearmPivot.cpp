#include "FirearmPivot.h"

#include "SceneRenderTargetParameters.h"
#include "Firearms/FirearmBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UFirearmPivot::UFirearmPivot() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UFirearmPivot::BeginPlay() {
	Super::BeginPlay();
	Equip(Firearm);
	Target = Cast<USceneComponent>(GetOwner()->AddComponentByClass(USceneComponent::StaticClass(), false, GetOwner()->GetTransform(), true));
}

void UFirearmPivot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	TryFire();
	DeltaTime = FMath::Clamp(DeltaTime, 0.f, 1.f/30);
	// ClampVelocities();
	UpdateState(DeltaTime);
	Resist(DeltaTime);
}

bool UFirearmPivot::TryFire() {
	return Firearm ? Firearm->TryFire() : false;
}

bool UFirearmPivot::Equip(class AFirearmBase* InFirearm) {
	if (!InFirearm)
		return false;
	Firearm = InFirearm;
	FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepRelativeTransform;
	Rules.bWeldSimulatedBodies = true;
	Firearm->Root->AttachToComponent(this, Rules);
	Firearm->SetActorRotation(GetComponentRotation());
	FVector Offset = GetComponentLocation() - Firearm->Pivot->GetComponentLocation();
	Firearm->AddActorWorldOffset(Offset);
	return true;
}

void UFirearmPivot::AddImpulse(FVector Impulse) {
	FVector Offset = Firearm->GetActorLocation() - Firearm->Pivot->GetComponentLocation();
	FVector AngularImpulse = Offset.Cross(Impulse) / 100.f;
	// float Rest = Impulse.Length() - AngularImpulse.Length();
	FVector LinearImpulse = Impulse;
	LinearVelocity+=LinearImpulse;
	AngularVelocity+=AngularImpulse;
}

void UFirearmPivot::UpdateState(float DeltaSeconds) {
	// if (DeltaSeconds * AngularVelocity.Length() > 20)
	FQuat DeltaRotation = FQuat::MakeFromRotationVector(AngularVelocity * DeltaSeconds);
	FVector Offset = Firearm->Pivot->GetComponentLocation() - Firearm->Root->GetComponentLocation();
	Firearm->Root->AddWorldOffset(Offset);
	Firearm->Root->AddWorldRotation(DeltaRotation);
	Firearm->Root->AddWorldOffset(-Offset);
	Firearm->Root->AddWorldOffset(LinearVelocity * DeltaSeconds);
}

void UFirearmPivot::Resist(float DeltaSeconds) {
	ResistLinear(DeltaSeconds);
	ResistAngular(DeltaSeconds);
}

void UFirearmPivot::ResistLinear(float DeltaSeconds) {
	FVector DeltaPosition = GetComponentLocation() - Firearm->Pivot->GetComponentLocation();
	FVector DeltaVelocity = -LinearVelocity;
	FVector Acceleration = LinearProportional * DeltaPosition + LinearDerivative * DeltaVelocity;
	LinearVelocity += Acceleration * DeltaSeconds;
}

void UFirearmPivot::ResistAngular(float DeltaSeconds) {
	FVector InertiaVector = Firearm->Root->BodyInstance.GetBodyInertiaTensor();
	FMatrix InertiaTensor = FMatrix::Identity;
	InertiaTensor.M[0][0] = InertiaVector.X;
	InertiaTensor.M[1][1] = InertiaVector.Y;
	InertiaTensor.M[2][2] = InertiaVector.Z;

	FQuat DeltaRotation = (Firearm->GetActorRotation() - GetComponentRotation()).Quaternion();
	FVector Torque = -Firearm->GetWeight() * (AngularProportional * DeltaRotation.ToRotationVector() + AngularDerivative * AngularVelocity);
	AngularVelocity += Torque * DeltaSeconds;
}


