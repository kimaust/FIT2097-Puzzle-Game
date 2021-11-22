// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHealthPack::AHealthPack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	HealAmount = 20;
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow yellow.
	FLinearColor Color = 0.2f * FLinearColor::Yellow * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString AHealthPack::Interact_Implementation()
{
	auto Character = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character) {
		Character->Heal(HealAmount);
		Destroy();
	}
	return FString(TEXT("You've used the health pack."));
}

FString AHealthPack::GetName_Implementation()
{
	return FString(TEXT("Health Pack"));
}

FString AHealthPack::GetDescription_Implementation()
{
	return FString(TEXT("Restores ")) + FString::SanitizeFloat(HealAmount) + FString(TEXT(" health points."));
}

FString AHealthPack::GetInteractDescription_Implementation()
{
	return FString(TEXT("Press E key to use"));
}