// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_Run.h"
#include "BaseCharacter.h"
#include "HumanAIController.h"
#include "BanditKingAIController.h"

UBTTask_Run::UBTTask_Run()
{
	bNotifyTick = true;
	NodeName = TEXT("Run");
}

EBTNodeResult::Type UBTTask_Run::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;

	character->Run(1.f);

	return EBTNodeResult::InProgress;
}

void UBTTask_Run::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	auto character = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());

	if (character->GetAnimState() != AnimState::ATTACKING)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}