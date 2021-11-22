// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingArea.h"

#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHealingArea::AHealingArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the mesh and the trigger area for the healing area.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(BaseMesh);

	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AHealingArea::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the component begin/end overlap event.
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AHealingArea::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &AHealingArea::OnComponentEndOverlap);
}

// Called every frame
void AHealingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealingArea::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		// Store player reference and set up the timer to heal the player every 0.25 seconds.
		Player = Character;
		GetWorldTimerManager().SetTimer(HealTimer, this, &AHealingArea::Heal, 0.25, true);
	}
}

void AHealingArea::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		GetWorldTimerManager().ClearTimer(HealTimer);
		Player = nullptr;
	}
}

void AHealingArea::Heal()
{	
	if (!Player) return;

	const float HealAmount = 1;
	Player->Heal(HealAmount);
}