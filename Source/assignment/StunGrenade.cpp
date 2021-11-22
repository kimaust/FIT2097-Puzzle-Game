// Fill out your copyright notice in the Description page of Project Settings.


#include "StunGrenade.h"

#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStunGrenade::AStunGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AStunGrenade::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AStunGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow yellow.
	FLinearColor Color = 0.2 * FLinearColor::Yellow * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString AStunGrenade::Interact_Implementation()
{
	auto Character = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character) {
		Character->AddItem("StunGrenade");
		Destroy();
	}
	return FString("You've obtained the stun grenade.");
}

FString AStunGrenade::GetName_Implementation()
{
	return FString("Stun Grenade");
}

FString AStunGrenade::GetDescription_Implementation()
{
	return FString("Stuns enemies temporarily when hit.");
}

FString AStunGrenade::GetInteractDescription_Implementation()
{
	return FString("Press E key to pick up");
}