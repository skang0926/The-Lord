// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KnightDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTService_KnightDetect : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_KnightDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
};
