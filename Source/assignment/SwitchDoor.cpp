// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchDoor.h"

ASwitchDoor::ASwitchDoor() : Super()
{
	IsLocked = true;
}

void ASwitchDoor::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the activation of door switch to allow unlocking the door remotely.
	if (DoorSwitch) {
		DoorSwitch->SwitchEventDispatcher.AddDynamic(this, &ASwitchDoor::Unlock);
	}
}

FString ASwitchDoor::Interact_Implementation()
{
	FString Message = "";
	
	if (IsLocked) {
		Message = "The door is locked.";
	} else {
		Message = Super::Interact_Implementation();
	}
	return Message;
}