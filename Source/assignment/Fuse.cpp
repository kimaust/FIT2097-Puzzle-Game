// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuse.h"
#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFuse::AFuse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AFuse::BeginPlay()
{
	Super::BeginPlay();

	Material = BaseMesh->GetMaterial(0);
	MaterialInstanceDynamic = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AFuse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MaterialInstanceDynamic) return;

	// Update the material emissive color to glow yellow.
	FLinearColor Color = 0.2 * FLinearColor::Yellow * FMath::Sin(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	MaterialInstanceDynamic->SetVectorParameterValue("EmissiveColor", Color);
}

FString AFuse::Interact_Implementation()
{
	auto Character = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character) {
		Character->AddItem("Fuse");
		Destroy();
	}
	return FString("You've obtained the fuse.");
}

FString AFuse::GetName_Implementation()
{
	return FString("Fuse");
}

FString AFuse::GetDescription_Implementation()
{
	return FString("Attach to the power box to provide a power.");
}

FString AFuse::GetInteractDescription_Implementation()
{
	return FString("Press E key to pick up");
}