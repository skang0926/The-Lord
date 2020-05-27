// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_GruxAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTDecorator_GruxAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTDecorator_GruxAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const override;
};
