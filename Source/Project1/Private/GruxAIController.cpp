// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AGruxAIController::homePosKey(TEXT("HomePos"));
const FName AGruxAIController::patrolPosKey(TEXT("PatrolPos"));
const FName AGruxAIController::TargetKey(TEXT("Target"));
const FName AGruxAIController::AttackCount(TEXT("AttackCount"));


AGruxAIController::AGruxAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/AI/BB_Grux.BB_Grux"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/AI/BT_Grux.BT_Grux"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}



void AGruxAIController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);


	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(homePosKey, inPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogClass, Warning, TEXT("coldn't run behavior tree"))
		}
	}
}


void AGruxAIController::OnUnPossess()
{
	Super::OnUnPossess();
}