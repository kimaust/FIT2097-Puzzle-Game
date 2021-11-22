// Copyright Epic Games, Inc. All Rights Reserved.

#include "assignmentCharacter.h"

#include "ActiveStunGrenade.h"
#include "assignmentProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

#include "Interactable.h"
#include "assignmentGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AassignmentCharacter

AassignmentCharacter::AassignmentCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	Health = 100;
	MaxHealth = 100;
	IsThrowingGrenade = false;
	GrenadeSpeed = 2500;
}

void AassignmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	GameMode = Cast<AassignmentGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FirstPersonCameraComponent->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	FirstPersonCameraComponent->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	FirstPersonCameraComponent->PostProcessSettings.DepthOfFieldFstop = 0.3;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AassignmentCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("LeftButtonClick", IE_Pressed, this, &AassignmentCharacter::OnLeftButtonPressed);
	PlayerInputComponent->BindAction("LeftButtonClick", IE_Released, this, &AassignmentCharacter::OnLeftButtonReleased);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AassignmentCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AassignmentCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AassignmentCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AassignmentCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AassignmentCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AassignmentCharacter::Use);
}

void AassignmentCharacter::OnLeftButtonPressed()
{
	// For now, if the player has a stun grenade, throw it if the left mouse button is pressed.
	// An equipment system would be more appropriate once built to call Use() on the equipped item.
	if (HasItem("StunGrenade")) {
		IsThrowingGrenade = true;
	}
}

void AassignmentCharacter::OnLeftButtonReleased()
{	
	if (IsThrowingGrenade) {
		const FVector GrenadeVelocity = UKismetMathLibrary::GetForwardVector(GetControlRotation()) * GrenadeSpeed;
		const FVector GrenadeThrowOffset = GetActorForwardVector() * 100 + GetActorUpVector() * 90;
		const FVector GrenadeStartLocation = GetActorLocation() + GrenadeThrowOffset;

		// Spawn a grenade and set its velocity to launch to the direction the player is looking at.
		FTransform GrenadeTransform(FRotator::ZeroRotator, GrenadeStartLocation);
		auto Grenade = GetWorld()->SpawnActorDeferred<AActiveStunGrenade>(
			AActiveStunGrenade::StaticClass(),
			GrenadeTransform);
		Grenade->BaseMesh->SetPhysicsLinearVelocity(GrenadeVelocity);
		UGameplayStatics::FinishSpawningActor(Grenade, GrenadeTransform);

		RemoveItem("StunGrenade");
		IsThrowingGrenade = false;
	}
}

void AassignmentCharacter::OnFire()
{
	/*
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AassignmentProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AassignmentProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	*/
}

void AassignmentCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AassignmentCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AassignmentCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AassignmentCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AassignmentCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AassignmentCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AassignmentCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AassignmentCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AassignmentCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AassignmentCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AassignmentCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AassignmentCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

bool AassignmentCharacter::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel,
	bool ReturnPhysMat)
{
	FCollisionQueryParams TraceParams(FName(TEXT("Character Trace")), true, ActorsToIgnore[0]);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	HitOut = FHitResult(ForceInit);
	World->LineTraceSingleByChannel(HitOut, Start, End, CollisionChannel, TraceParams);

	return HitOut.GetActor() != NULL;
}

AActor* AassignmentCharacter::CallMyTrace(float TraceDistance)
{
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	const FVector End = Start + ForwardVector * TraceDistance;

	FHitResult HitData(ForceInit);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false);
	return HitData.GetActor();
}

bool AassignmentCharacter::ImplementsInteractable(AActor* Interactable)
{
	return Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass());
}

void AassignmentCharacter::ExecuteInteractable(AActor* Interactable)
{
	if (ImplementsInteractable(Interactable)) {
		FString InteractNotification = IInteractable::Execute_Interact(Interactable);
		GameMode->SetInteractNotification(InteractNotification);
	}
}

void AassignmentCharacter::Use()
{
	const float TraceDistance = 256;
	AActor* Interactable = CallMyTrace(TraceDistance);
	if (ImplementsInteractable(Interactable)) {
		ExecuteInteractable(Interactable);
	}
}

void AassignmentCharacter::TraceInteractable()
{
	const float TraceDistance = 256;
	AActor* Interactable = CallMyTrace(TraceDistance);

	// Set up the interactable information to display.
	FString Name, Description, InteractDescription;
	if (ImplementsInteractable(Interactable)) {
		Name = IInteractable::Execute_GetName(Interactable);
		Description = IInteractable::Execute_GetDescription(Interactable);
		InteractDescription = IInteractable::Execute_GetInteractDescription(Interactable);
	}
	GameMode->SetInteractableInfo(Name, Description, InteractDescription);
}

void AassignmentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceInteractable();
	UpdateScreenPP();
	
	if (IsThrowingGrenade) {
		DrawGrenadePath();
	}
}

void AassignmentCharacter::DrawGrenadePath()
{
	const float GrenadeSize = 3;
	const float SimulationTime = 1;
	const FVector GrenadeThrowOffset = GetActorForwardVector() * 100 + GetActorUpVector() * 90;
	const FVector DrawStartPosition = GetActorLocation() + GrenadeThrowOffset;
	const FVector GrenadeLaunchVelocity = UKismetMathLibrary::GetForwardVector(GetControlRotation()) * GrenadeSpeed;

	FPredictProjectilePathParams GrenadePathParams(
		GrenadeSize, DrawStartPosition, GrenadeLaunchVelocity, SimulationTime);
	GrenadePathParams.ActorsToIgnore = TArray<AActor*>{ this };
	GrenadePathParams.bTraceWithCollision = true;
	GrenadePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	GrenadePathParams.DrawDebugTime = 1;
	GrenadePathParams.DrawDebugType = EDrawDebugTrace::Type::ForOneFrame;
	GrenadePathParams.SimFrequency = 25;

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(this, GrenadePathParams, Result);
}

void AassignmentCharacter::UpdateScreenPP()
{
	// Update the focal distance of the camera to the hit object to make depth-of-field effect.
	const float TraceDistance = 10000;
	const auto Actor = CallMyTrace(TraceDistance);
	if (Actor) {
		FirstPersonCameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = 
		 	FVector::Dist(FirstPersonCameraComponent->GetComponentLocation(), Actor->GetActorLocation());
	}
	// Update the camera post processing material to display blood on screen based on health.
	const float HealthRatio = Health / MaxHealth;
	const float BloodAlphaValue = 1 - (HealthRatio > 0.7f ? 1 : HealthRatio + 0.3);
	UKismetMaterialLibrary::SetScalarParameterValue(
		GetWorld(), ScreenPPMatParamCollection, "ScreenBloodAlpha", BloodAlphaValue);
}

float AassignmentCharacter::GetHealth()
{
	return Health;
}

float AassignmentCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void AassignmentCharacter::SetHealth(float NewHealth)
{
	// Bound the health to [0, MaxHealth] and dispatch health update event.
	Health = FMath::Min(FMath::Max(0.0f, NewHealth), MaxHealth);
	HealthEventDispatcher.Broadcast(Health);
}

void AassignmentCharacter::Heal(float Amount)
{
	SetHealth(Health + Amount);
}

void AassignmentCharacter::Damage(float Damage)
{
	SetHealth(Health - Damage);
}

void AassignmentCharacter::AddItem(FString Item)
{
	Items.Add(Item);
}

bool AassignmentCharacter::HasItem(FString Item)
{
	return Items.Contains(Item);
}

void AassignmentCharacter::RemoveItem(FString Item)
{
	Items.RemoveSingle(Item);
}