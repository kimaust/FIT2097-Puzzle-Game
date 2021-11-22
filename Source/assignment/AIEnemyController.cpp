// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"

#include "Door.h"
#include "assignmentCharacter.h"
#include "assignmentGameMode.h"

AAIEnemyController::AAIEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Default sight sense values.
	SightRadius = 4000;
	SightAge = 5;
	LoseSightRadius = SightRadius + 500.0f;
	FieldOfView = 90;

	// Set sight configuration.
	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);
	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);

	Player = nullptr;
	CurrentPatrolRouteIndex = 0;
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	if (!BlackboardData) return;
	if (!ensure(BehaviorTree)) return;

	UseBlackboard(BlackboardData, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnSensesUpdated);
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AAIEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if there is an unlocked door blocking the path and open it.
	auto Door = Cast<ADoor>(Trace());
	if (Door && !Door->IsOpened() && Door->IsUnlocked()) {
		Door->Open();
	}
	// Update player location on each tick.
	if (Player) {
		BlackboardComponent->SetValueAsVector("PlayerLocation", Player->GetActorLocation());
	}
}

FRotator AAIEnemyController::GetControlRotation() const
{
	if (!GetPawn()) return FRotator::ZeroRotator;
	
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAIEnemyController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TempPawn = Cast<APawn>(UpdatedActor);
	if (TempPawn && TempPawn->IsPlayerControlled()) {
		if (Stimulus.WasSuccessfullySensed()) {
			// Set the player reference and update blackboard variables to chase the player.
			Player = TempPawn;
			BlackboardComponent->SetValueAsBool("CanChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerLocation", Player->GetActorLocation());
		} else {
			// Clear the player reference and the variable when the player is not sensed.
			Player = nullptr;
			BlackboardComponent->ClearValue("CanChasePlayer");
		}
	}
}

void AAIEnemyController::GeneratePatrolLocation()
{
	if (PatrolRoutes.Num() == 0) return;
	
	// Cycle through defined patrol routes.
	FVector NextLocation = PatrolRoutes[CurrentPatrolRouteIndex++];
	CurrentPatrolRouteIndex = CurrentPatrolRouteIndex % PatrolRoutes.Num();
	BlackboardComponent->SetValueAsVector("PatrolLocation", NextLocation);
}

void AAIEnemyController::GenerateRandomLocation()
{
	if (NavigationSystem) {
		FNavLocation NextLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), NavigableRadius, NextLocation);
		BlackboardComponent->SetValueAsVector("PatrolLocation", NextLocation.Location);
	}
}

void AAIEnemyController::AttackPlayer()
{
	if (!Player) return;

	const float AttackDamage = 1;
	auto PlayerCharacter = Cast<AassignmentCharacter>(Player);
	PlayerCharacter->Damage(AttackDamage);
}

void AAIEnemyController::Stun(float Duration)
{
	BlackboardComponent->SetValueAsBool("IsStunned", true);
	BlackboardComponent->SetValueAsFloat("StunDuration", Duration);

	// Set up a timer to unstun after the specified duration.
	if (Duration > 0) {
		GetWorldTimerManager().SetTimer(StunTimer, this, &AAIEnemyController::Unstun, Duration);
	}
}

void AAIEnemyController::Unstun()
{
	GetWorldTimerManager().ClearTimer(StunTimer);
	BlackboardComponent->SetValueAsBool("IsStunned", false);
}

AActor* AAIEnemyController::Trace()
{
	const auto AICharacter = GetPawn();
	if (!AICharacter) return nullptr;

	const float TraceDistance = 256;
	const FVector Start = AICharacter->GetActorLocation();
	const FVector ForwardVector = AICharacter->GetActorForwardVector();
	const FVector End = Start + ForwardVector * TraceDistance;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AICharacter);

	FHitResult HitData(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("AI Trace")), true, ActorsToIgnore[0]);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	HitData = FHitResult(ForceInit);
	GetWorld()->LineTraceSingleByChannel(HitData, Start, End, ECC_Visibility, TraceParams);

	return HitData.GetActor();
}