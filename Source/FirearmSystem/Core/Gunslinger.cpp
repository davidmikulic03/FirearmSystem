// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunslinger.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FirearmSystem/Firearms/FirearmPivot.h"
#include "Kismet/GameplayStatics.h"


AGunslinger::AGunslinger() {
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	FirearmPivot = CreateDefaultSubobject<UFirearmPivot>("Firearm Pivot");
	FirearmPivot->SetupAttachment(Camera);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGunslinger::BeginMoving(const FInputActionValue& Value) {
	OnBeginMoving();
}

void AGunslinger::Move(const FInputActionValue& Value) {
	const FVector Input = FVector(Value.Get<FVector2d>(), 0.f);
	const FRotator Rotation = Controller->GetControlRotation();
	const FVector GlobalInput = Rotation.RotateVector(Input);
	AddMovementInput(GlobalInput);

}

void AGunslinger::FinishMoving(const FInputActionValue& Value) {
	OnFinishMoving();
	
}

void AGunslinger::BeginPanning(const FInputActionValue& Value) {
	OnBeginPanning();
}

void AGunslinger::Pan(const FInputActionValue& Value) {
	const FVector2d Input = Value.Get<FVector2d>();
        
	AddControllerPitchInput(-Input.Y);
	AddControllerYawInput(Input.X);
}

void AGunslinger::FinishPanning(const FInputActionValue& Value) {
	OnFinishPanning();
}

void AGunslinger::StartCrouch(const FInputActionValue& Value) {
	GetCharacterMovement()->bWantsToCrouch = true;
}

void AGunslinger::StopCrouch(const FInputActionValue& Value) {
	GetCharacterMovement()->bWantsToCrouch = false;
}

void AGunslinger::SingleFire(const FInputActionValue& Value) {
	FirearmPivot->TryFire();
}

void AGunslinger::StartFiring(const FInputActionValue& Value) {
	bIsFiring = true;
}

void AGunslinger::StopFiring(const FInputActionValue& Value) {
	bIsFiring = false;
}

void AGunslinger::BeginPlay()
{
	Super::BeginPlay();

	if (auto f = Cast<AFirearm>(UGameplayStatics::GetActorOfClass(GetWorld(), AFirearm::StaticClass()))) {
		FirearmPivot->Equip(f);
		SumResistParams(f->TruePivot->ResistParams);
	}
}

void AGunslinger::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsFiring)
		FirearmPivot->TryFire();
}

void AGunslinger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	auto PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(CharacterContext, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	Input->BindAction(MoveAction, ETriggerEvent::Started, this, &AGunslinger::BeginMoving);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGunslinger::Move);
	Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGunslinger::FinishMoving);

	Input->BindAction(PanAction, ETriggerEvent::Started, this, &AGunslinger::BeginPanning);
	Input->BindAction(PanAction, ETriggerEvent::Triggered, this, &AGunslinger::Pan);
	Input->BindAction(PanAction, ETriggerEvent::Completed, this, &AGunslinger::FinishPanning);
	
	Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGunslinger::StartCrouch);
	Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AGunslinger::StopCrouch);
	
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	
	Input->BindAction(ManualFireAction, ETriggerEvent::Triggered, this, &AGunslinger::SingleFire);
	Input->BindAction(AutomaticFireAction, ETriggerEvent::Started, this, &AGunslinger::StartFiring);
	Input->BindAction(AutomaticFireAction, ETriggerEvent::Completed, this, &AGunslinger::StopFiring);
}

void AGunslinger::SumResistParams(struct FRecoilResistParams Other) {
	
	if(Other.RecoilRandomness >= 0) FirearmPivot->ResistParams.RecoilRandomness = FirearmPivot->ResistParams.RecoilRandomness + Other.RecoilRandomness;
	else FirearmPivot->ResistParams.RecoilRandomness = FirearmPivot->ResistParams.RecoilRandomness * FMath::Exp(Other.RecoilRandomness / FirearmPivot->ResistParams.RecoilRandomness);
        
	if(Other.LinearProportional >= 0) FirearmPivot->ResistParams.LinearProportional = FirearmPivot->ResistParams.LinearProportional + Other.LinearProportional;
	else FirearmPivot->ResistParams.LinearProportional = FirearmPivot->ResistParams.LinearProportional * FMath::Exp(Other.LinearProportional / FirearmPivot->ResistParams.LinearProportional);
        
	if(Other.LinearDerivative >= 0) FirearmPivot->ResistParams.LinearDerivative = FirearmPivot->ResistParams.LinearDerivative + Other.LinearDerivative;
	else FirearmPivot->ResistParams.LinearDerivative = FirearmPivot->ResistParams.LinearDerivative * FMath::Exp(Other.LinearDerivative / FirearmPivot->ResistParams.LinearDerivative);

	if(Other.AngularProportional >= 0) FirearmPivot->ResistParams.AngularProportional = FirearmPivot->ResistParams.AngularProportional + Other.AngularProportional;
	else FirearmPivot->ResistParams.AngularProportional = FirearmPivot->ResistParams.AngularProportional * FMath::Exp(Other.AngularProportional / FirearmPivot->ResistParams.AngularProportional);
        
	if(Other.AngularDerivative >= 0) FirearmPivot->ResistParams.AngularDerivative = FirearmPivot->ResistParams.AngularDerivative + Other.AngularDerivative;
	else FirearmPivot->ResistParams.AngularDerivative = FirearmPivot->ResistParams.AngularDerivative * FMath::Exp(Other.AngularDerivative / FirearmPivot->ResistParams.AngularDerivative);
}

