// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Switch.h"
#include "FlipSwitch.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AFlipSwitch : public ASwitch
{
	GENERATED_BODY()
public:
	virtual FString Interact_Implementation() override;
	virtual FString GetName_Implementation() override;
	virtual FString GetDescription_Implementation() override;
	virtual FString GetInteractDescription_Implementation() override;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Flip Switch")
	UMaterial* OnMaterial;

	UPROPERTY(EditAnywhere, Category = "Flip Switch")
	UMaterial* OffMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flip Switch")
	bool IsOn;
};