// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create the base mesh.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	IsOpen = false;
	IsLocked = false;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = BaseMesh->GetComponentRotation();
	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocked) return;
	
	// Rotate the door based on whether it is opened.
	const FRotator RotateDeg = IsOpen ? FRotator(0, 90, 0) : FRotator(0, 0, 0);
	const FRotator Rotator = FMath::Lerp(BaseMesh->GetComponentRotation(), InitialRotation + RotateDeg, 0.05f);
	BaseMesh->SetRelativeRotation(Rotator);

	if (!MaterialInstanceDynamic) return;
	
	// Update the material emissive color to glow white when unlocked.
	const FLinearColor Color = FLinearColor::White * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString ADoor::Interact_Implementation()
{
	IsOpen = !IsOpen;
	return FString("");
}

FString ADoor::GetName_Implementation()
{
	return FString("Door");
}

FString ADoor::GetDescription_Implementation()
{
	return FString("It's a door.");
}

FString ADoor::GetInteractDescription_Implementation()
{
	return FString("Press E key to open");
}

void ADoor::Unlock()
{
	IsLocked = false;
}

void ADoor::Open()
{
	IsOpen = true;
	Unlock();
}

bool ADoor::IsUnlocked()
{
	return !IsLocked;
}

bool ADoor::IsOpened()
{
	return IsOpen;
}