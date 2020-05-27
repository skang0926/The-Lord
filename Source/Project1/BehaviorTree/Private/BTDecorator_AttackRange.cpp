// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTDecorator_AttackRange.h"
#include "HumanAIController.h"
#include "HumanCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("AttackRange");
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(ownerComp, nodeMemory);

	auto controllingPawn = ownerComp.GetAIOwner()->GetPawn();
	if (controllingPawn == nullptr)
		return false;

	auto target = dynamic_cast<ABaseCharacter*>(ownerComp.GetBlackboardComponent()->GetValueAsObject(AHumanAIController::TargetKey));

	if (target == nullptr)
		return false;

	FVector vDir = controllingPawn->GetActorLocation() - target->GetActorLocation();
	vDir = vDir.GetSafeNormal();


	FVector forwardVec = controllingPawn->GetActorForwardVector();
	float radianForwardVec = FVector::DotProduct(forwardVec, vDir);
	float degreeX = FMath::RadiansToDegrees(acos(radianForwardVec));


	bResult = (target->GetDistanceTo(controllingPawn) <= acceptableRadius && (degreeX > 160));
	return bResult;
}