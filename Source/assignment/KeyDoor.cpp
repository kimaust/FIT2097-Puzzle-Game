// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyDoor.h"

#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

AKeyDoor::AKeyDoor() : Super()
{
	IsLocked = true;
}

FString AKeyDoor::Interact_Implementation()
{
	FString Message = "";
	
	if (!IsLocked) {
		Message = Super::Interact_Implementation();
	} else {
		// Check if the character has the key and if so remove and unlock the door.
		auto Character = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!Character->HasItem("Key")) {
			Message = FString("You need a key to unlock the door.");
		} else {
			Character->RemoveItem("Key");
			Message = FString("You've unlocked the door.");
			IsLocked = false;
		}
	}
	return Message;
}
