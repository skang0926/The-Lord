// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GruxDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTService_GruxDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GruxDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	
};
