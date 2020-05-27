// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "AIController.h"
#include "KnightAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AKnightAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AKnightAIController();
	virtual void OnPossess(APawn* inPawn) override;
	virtual void OnUnPossess() override;

	static const FName ownerKey;
	static const FName homePosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;
};
