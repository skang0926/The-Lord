// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_BattleStep.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HumanAIController.h"
#include "BaseCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
UBTTask_BattleStep::UBTTask_BattleStep()
{
	NodeName = TEXT("BattleStep");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BattleStep::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	auto controllingPawn = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());

	float randValue = static_cast<float>((rand() + static_cast<int32>(controllingPawn->GetActorLocation().X)) % 3) - 1;
	
	if (bRandomForwardMove)
	{
		forwardMoveValue = randValue;
	}

	if (bRandomRightMove)
	{
		rightMoveValue = randValue;
	}

	if (bRandomRightMoveExclusionZero)
	{
		int flag = FMath::Rand() % 2;
		if (flag == 0)
			rightMoveValue = randValue;
		else
			rightMoveValue = -1.f;
	}


	
	remainMoveTime = FMath::FRandRange(FMath::Max(0.0f, moveTime - randomDeviation), (moveTime + randomDeviation));
	return EBTNodeResult::InProgress;
}


void UBTTask_BattleStep::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	auto controllingPawn = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (!controllingPawn) return;	

	controllingPawn->MoveForward(forwardMoveValue);
	controllingPawn->MoveRight(rightMoveValue);

	remainMoveTime -= deltaSeconds;
	
	const UBlackboardComponent* myBlackboard = ownerComp.GetBlackboardComponent();	

	FVector otherLocation;
	
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* keyValue = myBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* targetActor = Cast<AActor>(keyValue);
		otherLocation = targetActor->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		UBlackboardKeyType_Vector::FDataType keyValue = myBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		otherLocation = keyValue;
	}
	else return;
		
	if (FVector::Distance(otherLocation, controllingPawn->GetActorLocation()) < acceptableRadius) 
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);

	remainMoveTime -= deltaSeconds;

	if (remainMoveTime <= 0.0f)
	{
		// continue execution from this node		
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}
