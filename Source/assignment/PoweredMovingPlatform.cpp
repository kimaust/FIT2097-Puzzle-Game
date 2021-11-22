// Fill out your copyright notice in the Description page of Project Settings.


#include "PoweredMovingPlatform.h"

// Sets default values
APoweredMovingPlatform::APoweredMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	TravelDistance = 2000;
	TravelSpeed = 200;
	DirectionFactor = 1;
	IsActivated = false;
}

// Called when the game starts or when spawned
void APoweredMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (Switch) {
		Switch->SwitchEventDispatcher.AddDynamic(this, &APoweredMovingPlatform::Activate);
	}
	StartLocation = GetActorLocation();
}

// Called every frame
void APoweredMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsActivated) return;

	const FVector Direction = GetActorForwardVector() * DirectionFactor;
	const FVector DeltaLocation = Direction * TravelSpeed * DeltaTime;
	const FVector TargetLocation = StartLocation + Direction * TravelDistance;
	FVector NextLocation = GetActorLocation() + DeltaLocation;

	const float DistanceTolerance = 50;
	if (FVector::Dist(NextLocation, TargetLocation) < DistanceTolerance) {
		NextLocation = TargetLocation;
		StartLocation = TargetLocation;
		DirectionFactor *= -1;
	}
	SetActorLocation(NextLocation);
}

void APoweredMovingPlatform::Activate()
{
	IsActivated = !IsActivated;
}