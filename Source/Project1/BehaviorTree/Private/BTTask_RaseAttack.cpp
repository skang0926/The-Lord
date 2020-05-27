// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_RaseAttack.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RaseAttack::UBTTask_RaseAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("GruxRaseAttack");
}

EBTNodeResult::Type UBTTask_RaseAttack::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<AGrux*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->RaseAttack();
	
	ownerComp.GetBlackboardComponent()->SetValueAsInt(AGruxAIController::AttackCount, 0);

	return EBTNodeResult::InProgress;
}

void UBTTask_RaseAttack::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	auto character = dynamic_cast<AGrux*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}