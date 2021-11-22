// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"
#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorKey::ADoorKey()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ADoorKey::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ADoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow yellow.
	FLinearColor Color = 0.2f * FLinearColor::Yellow * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString ADoorKey::Interact_Implementation()
{
	auto Player = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player) {
		Player->AddItem("Key");
		Destroy();
	}
	return FString("You've obtained the key.");
}

FString ADoorKey::GetName_Implementation()
{
	return FString(TEXT("Door key"));
}

FString ADoorKey::GetDescription_Implementation()
{
	return FString(TEXT("It's a key for the door."));
}

FString ADoorKey::GetInteractDescription_Implementation()
{
	return FString(TEXT("Press E key to pick up."));
}