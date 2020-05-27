// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"



/**
 * 
 */


DECLARE_MULTICAST_DELEGATE(FOnChangeFactionCountDelegate)

UCLASS()
class PROJECT1_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMainGameState();

	void SetCharacterCount(TArray<class ABaseCharacter*>);
	TArray<int32> GetFactionCharacterCount();
	int32 GetCharacterCount();
	void DecreaseFactionCharacterCount(int32 factionType);
	
private:
	UPROPERTY(VisibleAnywhere)
		int32 characterCount;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> factionCharacterCount;	
};
