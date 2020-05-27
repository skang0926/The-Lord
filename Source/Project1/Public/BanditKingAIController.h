// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "AIController.h"
#include "BanditKingAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API ABanditKingAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABanditKingAIController();
	virtual void OnPossess(APawn* inPawn) override;
	virtual void OnUnPossess() override;

	static const FName homePosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;


};
