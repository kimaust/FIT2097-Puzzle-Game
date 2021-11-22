// Fill out your copyright notice in the Description page of Project Settings.


#include "PoweredFlipSwitch.h"

APoweredFlipSwitch::APoweredFlipSwitch() : Super()
{
	IsPowered = false;
}

void APoweredFlipSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	if (PowerSource) {
		PowerSource->OnPowered.AddDynamic(this, &APoweredFlipSwitch::OnPowerSupply);
	}
}

FString APoweredFlipSwitch::Interact_Implementation()
{
	FString Message = "";

	if (!IsPowered) {
		Message = FString("You need to supply a power first.");
	} else {
		Message = Super::Interact_Implementation();
	}
	return Message;
}

void APoweredFlipSwitch::OnPowerSupply()
{
	IsPowered = true;
}