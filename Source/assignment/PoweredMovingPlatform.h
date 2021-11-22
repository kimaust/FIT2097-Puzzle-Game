// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Switch.h"
#include "GameFramework/Actor.h"
#include "PoweredMovingPlatform.generated.h"

UCLASS()
class ASSIGNMENT_API APoweredMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoweredMovingPlatform();

	UFUNCTION()
	void Activate();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	ASwitch* Switch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	float TravelDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	float TravelSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	bool IsActivated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	FVector StartLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Powered Moving Platform")
	float DirectionFactor;
};
