// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "AIController.h"
#include "GruxAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AGruxAIController : public AAIController
{
	GENERATED_BODY()
	
	AGruxAIController();
	virtual void OnPossess(APawn* inPawn) override;
	virtual void OnUnPossess() override;

public:
	static const FName homePosKey;
	static const FName patrolPosKey;
	static const FName TargetKey;
	static const FName AttackCount;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

};
