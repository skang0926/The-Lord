// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_SkillA.h"
#include "BaseCharacter.h"
#include "HumanAIController.h"
#include "BanditKingAIController.h"

UBTTask_SkillA::UBTTask_SkillA()
{
	bNotifyTick = true;
	NodeName = TEXT("SkillA");
}

EBTNodeResult::Type UBTTask_SkillA::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->SkillA();

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillA::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}