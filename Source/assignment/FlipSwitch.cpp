// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipSwitch.h"

#include "Kismet/GameplayStatics.h"

void AFlipSwitch::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, OffMaterial);
}

void AFlipSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;
	
	// Update the material emissive color to glow white.
	const FLinearColor Color = 0.1 * FLinearColor::White * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString AFlipSwitch::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	IsOn = !IsOn;
	if (MaterialInstanceDynamic) {
		MaterialInstanceDynamic = BaseMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(
			0, IsOn ? OnMaterial : OffMaterial);
	}
	return FString("You've flipped the switch.");
}

FString AFlipSwitch::GetName_Implementation()
{
	return FString("Flip Switch");
}

FString AFlipSwitch::GetDescription_Implementation()
{
	return FString("");
}

FString AFlipSwitch::GetInteractDescription_Implementation()
{
	return FString("Press E key to flip the switch.");
}