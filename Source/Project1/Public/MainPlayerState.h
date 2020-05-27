// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDynamicGamePlayStateChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGamePlayStateChangedDelegate);

/**
 * 
 */

UENUM()
enum GamePlayState
{
	BEGIN UMETA(DisplayName = "BeginLevel"),
	BEGINTOPLAYING UMETA(DisplayName = "BeginToPlaying"),
	PLAYING UMETA(DisplayName = "Playing"),
	PLAYINGTOCOMPLETE  UMETA(DisplayName = "PlayingToComplete"),
	COMPLETE UMETA(DisplayName = "Complete")
};




UCLASS(BlueprintType, Blueprintable)
class PROJECT1_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()


	
public:
	AMainPlayerState();
	virtual void BeginPlay() override;

	void SetFactionCharacterCount();
	int32 GetEnemyCount() { return enemyCount; }

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDynamicGamePlayStateChangedDelegate OnDynamicGamePlayStateChanged;

	FOnGamePlayStateChangedDelegate OnGamePlayStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlayState, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<GamePlayState> gamePlayState = GamePlayState::BEGIN;

	UFUNCTION(BlueprintCallable, Category = State)
	GamePlayState GetGamePlayState() { return gamePlayState; }

	UFUNCTION(BlueprintCallable, Category = State)
	void SetGamePlayState(GamePlayState state);

	int GetGamePlayStateToInt();

private:
	UPROPERTY(VisibleAnywhere)
	int32 enemyCount;

	UPROPERTY(VisibleAnywhere)
	int32 friendlyCount;

	void InitGamePlayState();

	UPROPERTY(VisibleAnywhere)
	TArray<int32> factionCharacterCount;
};
