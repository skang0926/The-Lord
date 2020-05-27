// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAIController.h"
#include "BaseCharacter.h"
#include "UserPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AKnightAIController::ownerKey(TEXT("Owner"));
const FName AKnightAIController::TargetKey(TEXT("Target"));
const FName AKnightAIController::homePosKey(TEXT("HomePos"));


AKnightAIController::AKnightAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Characters/GhostLady_S2/AI/BB_KnightCharacter.BB_KnightCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Characters/GhostLady_S2/AI/BT_KnightCharacter.BT_KnightCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}



void AKnightAIController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);


	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(homePosKey, inPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogClass, Warning, TEXT("coldn't run behavior tree"))
		}

		auto playerController = dynamic_cast<AUserPlayerController*>(GetWorld()->GetFirstPlayerController());
		if (!playerController) return;

		auto ownerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());
		if (!ownerCharacter)
		{
			UE_LOG(LogClass, Warning, TEXT(" isn't vaild ownerCharacter"));
			return;
		}
		Blackboard->SetValueAsObject(AKnightAIController::ownerKey, ownerCharacter);
	}
}


void AKnightAIController::OnUnPossess()
{
	Super::OnUnPossess();
}