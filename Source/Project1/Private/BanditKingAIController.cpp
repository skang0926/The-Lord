// Fill out your copyright notice in the Description page of Project Settings.


#include "BanditKingAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABanditKingAIController::homePosKey(TEXT("HomePos"));
const FName ABanditKingAIController::TargetKey(TEXT("Target"));


ABanditKingAIController::ABanditKingAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Characters/InfinityBladeWarriors/AI/BB_BanditKing.BB_BanditKing"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Characters/InfinityBladeWarriors/AI/BT_BandtKing.BT_BandtKing"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}



void ABanditKingAIController::OnPossess(APawn* inPawn)
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


void ABanditKingAIController::OnUnPossess()
{
	Super::OnUnPossess();
}