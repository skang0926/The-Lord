// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "HumanAIController.h"
#include "HumanCharacter.h"
#include "Grux.h"
#include "GruxAIController.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	NodeName = TEXT("MoveToTarget");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
//	auto controllingPawn = dynamic_cast<AHumanCharacter*>(ownerComp.GetAIOwner()->GetPawn());	


	return EBTNodeResult::InProgress;
}


void UBTTask_MoveToTarget::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	const UBlackboardComponent* myBlackboard = ownerComp.GetBlackboardComponent();

	auto controllingPawn = dynamic_cast<AHumanCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	
	FVector vDir;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* keyValue = myBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* targetActor = Cast<AActor>(keyValue);
		vDir = targetActor->GetActorLocation() - controllingPawn->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		UBlackboardKeyType_Vector::FDataType keyValue = myBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		vDir = keyValue - controllingPawn->GetActorLocation();

	}
	else return;


	FVector vX(1.f, 0.f, 0.f);
	FVector vY(0.f, 1.f, 0.f);

	FVector vDirNormal = vDir.GetSafeNormal();
	float radiansX = FVector::DotProduct(vX, vDirNormal);
	float radiansY = FVector::DotProduct(vY, vDirNormal);

	float degreeX = FMath::RadiansToDegrees(acos(radiansX));
	float degreeY = FMath::RadiansToDegrees(acos(radiansY));
	if (degreeY > 90.f)
	{
		degreeX *= -1.f;
	}

	FRotator targetRotator = FRotationMatrix::MakeFromX(vDir).Rotator();	
	
	controllingPawn->SetActorRotation(FMath::RInterpTo(controllingPawn->GetActorRotation(), FRotator(0.f, targetRotator.Yaw, 0.f) , GetWorld()->GetDeltaSeconds(), 4.f));
	controllingPawn->MoveForward(1.f);
	
	if (vDir.Size() <= 200.f)
	{
		controllingPawn->MoveForward(0.f);
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
		controllingPawn->MoveForward(0.f);
	}
}