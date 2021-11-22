// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlipSwitch.h"
#include "GameFramework/Actor.h"
#include "LightBlock.generated.h"

UCLASS()
class ASSIGNMENT_API ALightBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightBlock();

	UFUNCTION(BlueprintCallable)
	void ToggleLight();
	
	UFUNCTION(BlueprintCallable)
	bool IsLightOn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light Block")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Light Block")
	UMaterial* LightOnMaterial;

	UPROPERTY(EditAnywhere, Category = "Light Block")
	UMaterial* LightOffMaterial;

	UPROPERTY(EditAnywhere, Category = "Light Block")
	AFlipSwitch* FlipSwitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light Block")
	bool IsOn;
	
	UMaterialInstanceDynamic* MaterialInstanceDynamic;
};