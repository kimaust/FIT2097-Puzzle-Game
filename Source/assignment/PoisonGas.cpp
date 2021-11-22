// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonGas.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APoisonGas::APoisonGas()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the base mesh and the area of effect for the poison gas.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	AreaOfEffect = CreateDefaultSubobject<UBoxComponent>(TEXT("Area Of Effect"));
	AreaOfEffect->SetupAttachment(BaseMesh);

	RootComponent = BaseMesh;

	Damage = 1;
	AttacksPerSecond = 2;
}

// Called when the game starts or when spawned
void APoisonGas::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	// Set up the event listeners for the component begin overlap/end.
	AreaOfEffect->OnComponentBeginOverlap.AddDynamic(this, &APoisonGas::OnComponentBeginOverlap);
	AreaOfEffect->OnComponentEndOverlap.AddDynamic(this, &APoisonGas::OnComponentEndOverlap);
}

// Called every frame
void APoisonGas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APoisonGas::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		// Set up the timer to damage the player every AttacksPerSecond.
		if (AttacksPerSecond != 0) {
			GetWorldTimerManager().SetTimer(
				PoisonTimer, 
				this,
				&APoisonGas::DamagePlayer,
				1.0f / AttacksPerSecond, 
				true);
		}
		Player = Character;
	}
}

void APoisonGas::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		// Clear the timer and player reference.
		if (AttacksPerSecond != 0) {
			GetWorldTimerManager().ClearTimer(PoisonTimer);
		}
		Player = nullptr;
	}
}

void APoisonGas::DamagePlayer()
{
	if (Player) Player->Damage(Damage);
}