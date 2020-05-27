// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTDecorator_AttackRange : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Node)
	float acceptableRadius;
public:
	UBTDecorator_AttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const override;

};
