// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerBox.h"
#include "FlipSwitch.h"
#include "PoweredFlipSwitch.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API APoweredFlipSwitch : public AFlipSwitch
{
	GENERATED_BODY()
public:
	APoweredFlipSwitch();
	
	UFUNCTION(BlueprintCallable)
	void OnPowerSupply();

	virtual FString Interact_Implementation() override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flip Switch")
	bool IsPowered;

	UPROPERTY(EditAnywhere, Category = "Flip Switch")
	APowerBox* PowerSource;
};
