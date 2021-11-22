// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratePatrolLocation.h"

#include "AIEnemyController.h"

EBTNodeResult::Type UGeneratePatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component) return EBTNodeResult::Failed;

	auto Controller = Cast<AAIEnemyController>(Component->GetOwner());
	if (!Controller) return EBTNodeResult::Failed;

	Controller->GeneratePatrolLocation();
	return EBTNodeResult::Succeeded;
}