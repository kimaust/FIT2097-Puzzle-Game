// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Switch.h"
#include "SwitchDoor.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ASwitchDoor : public ADoor
{
	GENERATED_BODY()
public:
	ASwitchDoor();

	virtual FString Interact_Implementation() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Door")
	ASwitch* DoorSwitch; 
};
