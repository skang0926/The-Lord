// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_SkillB.h"
#include "BaseCharacter.h"
#include "HumanAIController.h"
#include "BanditKingAIController.h"

UBTTask_SkillB::UBTTask_SkillB()
{
	bNotifyTick = true;
	NodeName = TEXT("SkillB");
}

EBTNodeResult::Type UBTTask_SkillB::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->SkillB();

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillB::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}