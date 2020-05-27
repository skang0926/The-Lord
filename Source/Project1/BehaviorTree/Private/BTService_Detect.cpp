// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTService_Detect.h"
#include "HumanAIController.h"
#include "HumanCharacter.h" 
#include "BaseCharacter.h"
#include "CharacterDataTable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	//	Super::UBTService();
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickNode(ownerComp, nodeMemory, deltaSeconds);

	auto controllingPawn = dynamic_cast<ABaseCharacter*>(ownerComp.GetAIOwner()->GetPawn());
	if (nullptr == controllingPawn) return;

	

	UWorld* world = controllingPawn->GetWorld();
	
	if (nullptr == world) return;


	if (controllingPawn->GetLockOnTarget())
	{
		ownerComp.GetBlackboardComponent()->SetValueAsObject(AHumanAIController::TargetKey, controllingPawn->GetLockOnTarget());
		return;
	}
	

	FVector Center = controllingPawn->GetActorLocation();
	float DetectRadius = 1000.0f;

	if (nullptr == world) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, controllingPawn);
	bool bResult = world->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (!bResult)
	{
		ownerComp.GetBlackboardComponent()->SetValueAsObject(AHumanAIController::TargetKey, nullptr);
#if DRAW_DEBUG
		DrawDebugSphere(world, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
#endif	
		return;
	}

	for (auto OverlapResult : OverlapResults)
	{
		ABaseCharacter* otherCharacter = dynamic_cast<ABaseCharacter*>(OverlapResult.GetActor());
		if (!otherCharacter) continue;

		if (otherCharacter->GetAnimState() == AnimState::DEAD)
			continue;

		if (otherCharacter->GetCharacterStatData().faction != controllingPawn->GetCharacterStatData().faction)
		{
			ownerComp.GetBlackboardComponent()->SetValueAsObject(AHumanAIController::TargetKey, otherCharacter);

			controllingPawn->SetLockOnTarget(otherCharacter);

#if DRAW_DEBUG
			DrawDebugSphere(world, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

			DrawDebugPoint(world, otherCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
			DrawDebugLine(world, controllingPawn->GetActorLocation(), otherCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
#endif		
			return;
		}
	}	


		
	//controllingPawn->LockOn();

	//ABaseCharacter* baseCharacter = controllingPawn->GetLockOnTarget();

	//if(baseCharacter && baseCharacter->GetController()->IsPlayerController())
	//{		
	//	ownerComp.GetBlackboardComponent()->SetValueAsObject(AHumanAIController::TargetKey, baseCharacter);
	//	return;
	//}

	//ownerComp.GetBlackboardComponent()->SetValueAsObject(AHumanAIController::TargetKey, nullptr);	
}