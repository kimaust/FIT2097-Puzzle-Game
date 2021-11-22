// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "assignmentCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "PoisonGas.generated.h"

UCLASS()
class ASSIGNMENT_API APoisonGas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoisonGas();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	void OnComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void DamagePlayer();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Poison Gas")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Poison Gas")
	UBoxComponent* AreaOfEffect;

	UPROPERTY(EditAnywhere, Category = "Poison Gas")
	UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, Category = "Poison Gas")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Poison Gas")
	float AttacksPerSecond;

	FTimerHandle PoisonTimer;
	AassignmentCharacter* Player;
};
