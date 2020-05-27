// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_FindPatrolPos.h"
#include "HumanAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}


EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(ownerComp, nodeMemory);

	auto controllingPawn = ownerComp.GetAIOwner()->GetPawn();

	if (nullptr == controllingPawn)
		return EBTNodeResult::Failed;
	
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(controllingPawn->GetWorld());

	if (nullptr == navSystem)
		return EBTNodeResult::Failed;

	FVector originPos = ownerComp.GetBlackboardComponent()->GetValueAsVector(AHumanAIController::homePosKey);

	FNavLocation nextPatrol;
	
	if (navSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 1000.f, nextPatrol))
	{
		ownerComp.GetBlackboardComponent()->SetValueAsVector(AHumanAIController::patrolPosKey, nextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}