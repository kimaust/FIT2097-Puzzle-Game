// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "assignmentCharacter.h"

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the mesh and the trigger area for the portal.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(BaseMesh);

	RootComponent = BaseMesh;
	TeleportLocation = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the component begin/end overlap event.
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &APortal::OnComponentEndOverlap);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortal::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Telport the player character to the specified location.
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		Character->SetActorLocation(TeleportLocation);
	}
}

void APortal::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}