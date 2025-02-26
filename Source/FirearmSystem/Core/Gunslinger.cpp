#include "Gunslinger.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FirearmSystem/Firearms/FirearmPivot.h"
#include "FirearmSystem/Firearms/WeightedContactPoint.h"
#include "Kismet/GameplayStatics.h"


AGunslinger::AGunslinger() {
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	FirearmPivot = CreateDefaultSubobject<UWeightedContactPoint>("Firearm Pivot");
	FirearmPivot->SetupAttachment(Camera);
	TruePivot = CreateDefaultSubobject<UFirearmPivot>("True Pivot");
	TruePivot->SetupAttachment(FirearmPivot);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGunslinger::BeginMoving(const FInputActionValue& Value) {
	OnBeginMoving();
}

void AGunslinger::Move(const FInputActionValue& Value) {
	const FVector Input = FVector(Value.Get<FVector2d>(), 0.f);
	const FRotator Rotation = Controller->GetControlRotation();
	FVector GlobalInput = Rotation.RotateVector(Input);
	AddMovementInput(GlobalInput.GetSafeNormal2D());

}

void AGunslinger::FinishMoving(const FInputActionValue& Value) {
	OnFinishMoving();
}

void AGunslinger::Pan(const FInputActionValue& Value) {
	const FVector2d Input = Value.Get<FVector2d>();
        
	AddControllerPitchInput(-Input.Y);
	AddControllerYawInput(Input.X);
}

void AGunslinger::StartCrouch(const FInputActionValue& Value) {
	GetCharacterMovement()->bWantsToCrouch = true;
}

void AGunslinger::StopCrouch(const FInputActionValue& Value) {
	GetCharacterMovement()->bWantsToCrouch = false;
}

void AGunslinger::SingleFire(const FInputActionValue& Value) {
	TruePivot->TryFire();
}

void AGunslinger::StartFiring(const FInputActionValue& Value) {
	TruePivot->SetIsIdle(false);
	bIsFiring = true;
}

void AGunslinger::StopFiring(const FInputActionValue& Value) {
	TruePivot->SetIsIdle(true);
	bIsFiring = false;
}

void AGunslinger::BeginPlay()
{
	Super::BeginPlay();
	TruePivot->SetOwner(this);
	if (auto f = Cast<AFirearm>(UGameplayStatics::GetActorOfClass(GetWorld(), AFirearm::StaticClass()))) {
		TruePivot->Equip(f);
		EvaluateTruePivot();
	}
}

void AGunslinger::EvaluateTruePivot() {
	checkf(FirearmPivot, TEXT("Firearm Pivot not properly initialized."))
	TruePivot->ResistParams = FirearmPivot->ResistParams;
	if (auto f = TruePivot->Firearm) {
		TruePivot->ResistParams = FRecoilResistParams::SoftNormalizedSum(TruePivot->ResistParams , f->GetResistParams());
		// f->ModifyPivot(TruePivot->CenterOfMass, FirearmPivot->LocationWeight);
	}
}

FVector AGunslinger::GetGunTargetDirection() const {
	if (!TruePivot->Firearm)
		return Camera->GetForwardVector();
	FVector Start =  Camera->GetComponentLocation();
	FVector End = Camera->GetComponentLocation() + Camera->GetForwardVector() * StandardTargetDistance;
	FHitResult Hit;
	TArray<AActor*> Ignore;
	Ignore.Init(TruePivot->Firearm->GetBarrelExit()->GetOwner(), 1);
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false,
		Ignore, EDrawDebugTrace::None, Hit, true);

	FVector BarrelLocation = TruePivot->Firearm->GetBarrelExit()->GetComponentLocation();
	FVector Displacement = (Hit.Location - BarrelLocation);
	if (bHit) {
		FVector Direction = Displacement.GetSafeNormal();
		float Dot = Direction | Camera->GetForwardVector();
		if (Dot > 0)
			return Direction;
		// else {
		// 	FVector Sideways = Camera->GetRightVector();
		// 	FVector BarrelOffset = BarrelLocation - Camera->GetComponentLocation();
		// 	if ((Hit.Normal | Camera->GetForwardVector()) < 0 && (BarrelOffset | Hit.Normal) < 0)
		// 		Sideways = -Sideways;
		// 	FVector AlongNormal = (Sideways.ProjectOnToNormal(Hit.Normal));
		// 	FVector AlongWall = (Sideways-AlongNormal).GetSafeNormal();
		// 	
		// 	// if ((AlongWall | Direction) < 0)
		// 	// 	AlongWall = -AlongWall;
		// 	return AlongWall;
		// }
	}
	return Camera->GetForwardVector().GetSafeNormal();
}

void AGunslinger::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsFiring)
		TruePivot->TryFire();
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

	Input->BindAction(PanAction, ETriggerEvent::Triggered, this, &AGunslinger::Pan);
	
	Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGunslinger::StartCrouch);
	Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AGunslinger::StopCrouch);
	
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	
	Input->BindAction(ManualFireAction, ETriggerEvent::Triggered, this, &AGunslinger::SingleFire);
	Input->BindAction(AutomaticFireAction, ETriggerEvent::Started, this, &AGunslinger::StartFiring);
	Input->BindAction(AutomaticFireAction, ETriggerEvent::Completed, this, &AGunslinger::StopFiring);
}


