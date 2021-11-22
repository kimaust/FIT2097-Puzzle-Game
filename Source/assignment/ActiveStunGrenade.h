// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "ActiveStunGrenade.generated.h"

UCLASS()
class ASSIGNMENT_API AActiveStunGrenade : public AActor
{
	GENERATED_BODY()
	
public:
	friend class AassignmentCharacter;
	
	// Sets default values for this actor's properties
	AActiveStunGrenade();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Explode();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Stun Grenade")
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Stun Grenade")
	float Duration;  // time it takes to explode in seconds

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Stun Grenade")
	float StunDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Stun Grenade")
	float ExplosionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Stun Grenade")
	UNiagaraSystem* ExplosionFX;

	FTimerHandle GrenadeTimer;
};