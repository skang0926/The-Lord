// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillB.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_SkillB : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_SkillB();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};
