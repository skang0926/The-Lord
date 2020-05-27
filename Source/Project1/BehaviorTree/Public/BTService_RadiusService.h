// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RadiusService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBTService_RadiusService : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_RadiusService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
};
