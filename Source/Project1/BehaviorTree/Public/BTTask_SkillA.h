// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillA.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_SkillA : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SkillA();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};
