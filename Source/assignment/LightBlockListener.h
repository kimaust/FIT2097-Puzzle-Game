// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwitchDoor.h"
#include "LightBlock.h"
#include "GameFramework/Actor.h"
#include "LightBlockListener.generated.h"

UCLASS()
class ASSIGNMENT_API ALightBlockListener : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightBlockListener();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light Block Listener")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Light Block Listener")
	TArray<ALightBlock*> LightBlocks;

	UPROPERTY(EditAnywhere, Category = "Light Block Listener")
	ADoor* Door;

	UPROPERTY(EditAnywhere, Category = "Light Block Listener")
	FString BinaryPasscode;
};