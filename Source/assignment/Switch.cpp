// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASwitch::ASwitch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	InteractMessage = "";
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow white.
	const FLinearColor Color = FLinearColor::White * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString ASwitch::Interact_Implementation()
{
	SwitchEventDispatcher.Broadcast();
	return FString(InteractMessage);
}

FString ASwitch::GetName_Implementation()
{
	return FString("Switch");
}

FString ASwitch::GetDescription_Implementation()
{
	return FString("");
}

FString ASwitch::GetInteractDescription_Implementation()
{
	return FString("Press E key to activate");
}