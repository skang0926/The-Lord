// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GruxAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_GruxAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_GruxAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};

