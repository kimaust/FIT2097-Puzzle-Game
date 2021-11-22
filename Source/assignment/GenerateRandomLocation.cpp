// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateRandomLocation.h"

#include "AIEnemyController.h"

EBTNodeResult::Type UGenerateRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component) return EBTNodeResult::Failed;

	auto Controller = Cast<AAIEnemyController>(Component->GetOwner());
	if (!Controller) return EBTNodeResult::Failed;

	Controller->GenerateRandomLocation();
	return EBTNodeResult::Succeeded;
}