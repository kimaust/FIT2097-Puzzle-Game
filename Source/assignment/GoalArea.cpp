// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalArea.h"
#include "assignmentGameMode.h"
#include "assignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoalArea::AGoalArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the mesh and the trigger area for the goal area.
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(BaseMesh);

	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AGoalArea::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the component begin/end overlap event.
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AGoalArea::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &AGoalArea::OnComponentEndOverlap);
}

// Called every frame
void AGoalArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalArea::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	auto Character = Cast<AassignmentCharacter>(OtherActor);
	if (Character) {
		// Set the game state to won.
		auto GameMode = Cast<AassignmentGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->SetHasWon(true);
	}
}

void AGoalArea::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}