// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AHumanAIController::homePosKey(TEXT("HomePos"));
const FName AHumanAIController::patrolPosKey(TEXT("PatrolPos"));
const FName AHumanAIController::TargetKey(TEXT("Target"));


AHumanAIController::AHumanAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Characters/GhostLady_S2/AI/BB_HumanCharacter.BB_HumanCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Characters/GhostLady_S2/AI/BT_HumanCharacter.BT_HumanCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}



void AHumanAIController::OnPossess(APawn* inPawn)
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


void AHumanAIController::OnUnPossess()
{	
	Super::OnUnPossess();
}