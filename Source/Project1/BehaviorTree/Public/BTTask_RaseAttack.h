// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RaseAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_RaseAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RaseAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};



