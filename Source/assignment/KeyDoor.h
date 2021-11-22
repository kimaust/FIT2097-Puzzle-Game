// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "KeyDoor.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AKeyDoor : public ADoor
{
	GENERATED_BODY()
public:
	AKeyDoor();
	
	virtual FString Interact_Implementation() override;
};
