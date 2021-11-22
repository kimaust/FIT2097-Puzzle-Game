// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	friend class AassignmentGameMode;
	
	AAIEnemyController();
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual FRotator GetControlRotation() const override;
	virtual void GeneratePatrolLocation();
	virtual void GenerateRandomLocation();
	virtual void AttackPlayer();
	virtual void Stun(float Duration);
	virtual void Unstun();

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UFUNCTION()
	AActor* Trace();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float SightRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float SightAge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float LoseSightRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float FieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBlackboardData* BlackboardData;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, Category = "AI Controller")
	float NavigableRadius;

	UPROPERTY(EditAnywhere, Category = "AI Controller")
	TArray<FVector> PatrolRoutes;

	APawn* Player;
	UNavigationSystemV1* NavigationSystem;

	int CurrentPatrolRouteIndex;

	FTimerHandle StunTimer;
};
