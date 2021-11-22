// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIEnemyCharacter.generated.h"

UCLASS()
class ASSIGNMENT_API AAIEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Enemy Character")
	float RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Enemy Character")
	float MovementSpeedFactor;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
