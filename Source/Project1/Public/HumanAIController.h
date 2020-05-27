// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "AIController.h"
#include "HumanAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AHumanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHumanAIController();
	virtual void OnPossess(APawn* inPawn) override;
	virtual void OnUnPossess() override;

	static const FName homePosKey;
	static const FName patrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
