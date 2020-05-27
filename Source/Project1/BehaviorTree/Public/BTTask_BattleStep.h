// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BattleStep.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTTask_BattleStep : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

	UBTTask_BattleStep();

	UPROPERTY(EditAnywhere, Category = Node)
	float forwardMoveValue;

	UPROPERTY(EditAnywhere, Category = Node)
	bool bRandomForwardMove;

	UPROPERTY(EditAnywhere, Category = Node)
	float rightMoveValue;

	UPROPERTY(EditAnywhere, Category = Node)
	bool bRandomRightMove;

	UPROPERTY(EditAnywhere, Category = Node)
	bool bRandomRightMoveExclusionZero;

	UPROPERTY(EditAnywhere, Category = Node)
	float moveTime;

	UPROPERTY(EditAnywhere, Category = Node)
	float randomDeviation;

	UPROPERTY(EditAnywhere, Category = Node)
	float acceptableRadius;

	float remainMoveTime;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds);
};
