// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_Attack.h"
#include "HumanCharacter.h"
#include "BaseCharacter.h"
#include "HumanAIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;
	
	character->Attack();
	character->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	
	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}