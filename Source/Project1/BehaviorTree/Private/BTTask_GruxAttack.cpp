// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_GruxAttack.h"
#include "Grux.h"
#include "BaseCharacter.h"
#include "GruxAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GruxAttack::UBTTask_GruxAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("GruxAttack");
}

EBTNodeResult::Type UBTTask_GruxAttack::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->Attack();
	int32 attackCount = ownerComp.GetBlackboardComponent()->GetValueAsInt(AGruxAIController::AttackCount);
	ownerComp.GetBlackboardComponent()->SetValueAsInt(AGruxAIController::AttackCount, ++attackCount);

	return EBTNodeResult::InProgress;
}

void UBTTask_GruxAttack::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	auto character = dynamic_cast<AGrux*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}