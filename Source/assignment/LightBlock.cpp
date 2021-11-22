// Fill out your copyright notice in the Description page of Project Settings.


#include "LightBlock.h"

// Sets default values
ALightBlock::ALightBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	IsOn = false;
}

// Called when the game starts or when spawned
void ALightBlock::BeginPlay()
{
	Super::BeginPlay();

	if (FlipSwitch) {
		FlipSwitch->SwitchEventDispatcher.AddDynamic(this, &ALightBlock::ToggleLight);
	}
	MaterialInstanceDynamic = BaseMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LightOffMaterial);
}

// Called every frame
void ALightBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightBlock::ToggleLight()
{
	IsOn = !IsOn;

	if (!MaterialInstanceDynamic) return;
	MaterialInstanceDynamic = BaseMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(
		0, IsOn ? LightOnMaterial : LightOffMaterial);
}

bool ALightBlock::IsLightOn()
{
	return IsOn;
}