// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToTarget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_MoveToTarget();



	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};
