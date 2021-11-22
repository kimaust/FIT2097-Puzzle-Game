// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerBox.h"

#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerBox::APowerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	IsPowered = false;
}

// Called when the game starts or when spawned
void APowerBox::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void APowerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow light blue.
	FLinearColor Color = 0.3 * FLinearColor(0.364f, 0.67f, 0.78f) * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString APowerBox::Interact_Implementation()
{
	FString Message = "";
	if (!IsPowered) {
		auto Player = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!Player->HasItem("Fuse")) {
			Message = "You need a fuse to provide a power.";
		} else {
			Player->RemoveItem("Fuse");
			Message = "You've attached the fuse.";
			IsPowered = true;
			OnPowered.Broadcast();
		}
	}
	return Message;
}

FString APowerBox::GetName_Implementation()
{
	return FString("Power Box");
}

FString APowerBox::GetDescription_Implementation()
{
	return FString("It's a power box for the switch.\nYou need a fuse to provide the power.");
}

FString APowerBox::GetInteractDescription_Implementation()
{
	return FString("Press E key to interact");
}