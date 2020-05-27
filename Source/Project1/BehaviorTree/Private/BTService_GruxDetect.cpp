// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Public/BTService_GruxDetect.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "HumanCharacter.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_GruxDetect::UBTService_GruxDetect()
{
	//	Super::UBTService();
	NodeName = TEXT("GruxDetect");
	Interval = 1.f;
}

void UBTService_GruxDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 1000.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AHumanCharacter* humanCharceter = Cast<AHumanCharacter>(OverlapResult.GetActor());

			if (humanCharceter)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruxAIController::TargetKey, humanCharceter);
#if DRAW_DEBUG
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, humanCharceter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), humanCharceter->GetActorLocation(), FColor::Blue, false, 0.2f);
#endif
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruxAIController::TargetKey, nullptr);
	}
#if DRAW_DEBUG
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
#endif
}
