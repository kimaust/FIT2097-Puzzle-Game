// Fill out your copyright notice in the Description page of Project Settings.


#include "LightBlockListener.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALightBlockListener::ALightBlockListener()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ALightBlockListener::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ALightBlockListener::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Listen for 3 light blocks to be turned-on in a specific way and opens the door if that's done.
	if (LightBlocks.Num() != BinaryPasscode.Len()) return;

	for (int i = 0; i < BinaryPasscode.Len(); ++i) {
		if (LightBlocks[i]->IsLightOn() != (BinaryPasscode[i] == '1')) return;
	}
	Door->Open();
	Destroy();
}