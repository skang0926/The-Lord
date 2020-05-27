// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_TurnToTarget.h"
#include "BaseCharacter.h"
#include "BanditKingAIController.h"
#include "HumanCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto owner = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (owner)
		return EBTNodeResult::Failed;

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABanditKingAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - owner->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	owner->SetActorRotation(FMath::RInterpTo(owner->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));


	return EBTNodeResult::Succeeded;
}


