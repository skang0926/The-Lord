// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTDecorator_GruxAttackRange.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "HumanCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_GruxAttackRange::UBTDecorator_GruxAttackRange()
{
	NodeName = TEXT("GruxAttackRange");
}

bool UBTDecorator_GruxAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(ownerComp, nodeMemory);

	auto controllingPawn = ownerComp.GetAIOwner()->GetPawn();
	if (controllingPawn == nullptr)
		return false;

	auto target = dynamic_cast<ABaseCharacter*>(ownerComp.GetBlackboardComponent()->GetValueAsObject(AGruxAIController::TargetKey));

	if (target == nullptr)
		return false;

	bResult = (target->GetDistanceTo(controllingPawn) <= 200.f);
	
	if (bResult)
	{
		FVector vDir = target->GetActorLocation() - controllingPawn->GetActorLocation();
		vDir = vDir.GetSafeNormal();


		FVector forwardVec = controllingPawn->GetActorForwardVector();;
		float radian = FVector::DotProduct(forwardVec, vDir);
		float degree = FMath::RadiansToDegrees(acos(radian));

		if (degree > 60.f)
		{
			bResult = false;
		}
	}	

	return bResult;
}