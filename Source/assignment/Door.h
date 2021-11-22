// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class ASSIGNMENT_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FString Interact();                 // Interacts with the object and returns interact result string.

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FString GetName();                  // Get interactable object name

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FString GetDescription();           // Get interactable object description

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	FString GetInteractDescription();   // Get interact description for the object.

	virtual FString Interact_Implementation() override;
	virtual FString GetName_Implementation() override;
	virtual FString GetDescription_Implementation() override;
	virtual FString GetInteractDescription_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void Unlock();

	UFUNCTION(BlueprintCallable)
	void Open();

	UFUNCTION(BlueprintCallable)
	bool IsOpened();
	
	UFUNCTION(BlueprintCallable)
	bool IsUnlocked();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool IsOpen;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool IsLocked;

	// Initial rotation applied to this object.
	FRotator InitialRotation;

	UMaterialInterface* Material;
	UMaterialInstanceDynamic* MaterialInstanceDynamic;
};
