// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTService_RadiusService.h"
#include "BanditKing.h"
#include "HumanCharacter.h"
#include "BanditKingAIController.h"
#include "CharacterDataTable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UserPlayerController.h"
#include "DrawDebugHelpers.h"

UBTService_RadiusService::UBTService_RadiusService()
{
	//	Super::UBTService();
	NodeName = TEXT("RadiusDetect");
	Interval = 1.f;
}

void UBTService_RadiusService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	auto controllingPawn = dynamic_cast<ABanditKing*>(OwnerComp.GetAIOwner()->GetPawn());

	if (controllingPawn->GetLockOnTarget()) return;

	UWorld* World = controllingPawn->GetWorld();
	FVector Center = controllingPawn->GetActorLocation();
	float DetectRadius = 1000.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, controllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ABaseCharacter* otherCharacter = dynamic_cast<ABaseCharacter*>(OverlapResult.GetActor());
			
			if (!(otherCharacter && controllingPawn)) break;
			if (otherCharacter->GetCharacterStatData().faction != controllingPawn->GetCharacterStatData().faction)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABanditKingAIController::TargetKey, otherCharacter);

				controllingPawn->SetLockOnTarget(otherCharacter);

				auto otherPlayerController = dynamic_cast<AUserPlayerController*>(otherCharacter->GetController());

				if (otherPlayerController)
				{
					otherPlayerController->SetEnemyHUD(controllingPawn->characterStat);
				}

#if DRAW_DEBUG
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, otherCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, controllingPawn->GetActorLocation(), otherCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
#endif			
				
				return;
			}	
		}
	}	

#if DRAW_DEBUG
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
#endif
}