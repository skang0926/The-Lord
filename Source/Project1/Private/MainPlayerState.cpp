// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "MainGameState.h"
#include "UserPlayerController.h"
#include "BaseCharacter.h"
#include "CharacterDataTable.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"

AMainPlayerState::AMainPlayerState()
{
	friendlyCount = 0;
	enemyCount = 0;
}

void AMainPlayerState::BeginPlay()
{	
	Super::BeginPlay();
	
	InitGamePlayState();



	SetFactionCharacterCount();
}

void AMainPlayerState::SetFactionCharacterCount()
{
	auto gameState = dynamic_cast<AMainGameState*>(GetWorld()->GetGameState());
	if (!gameState) return;

	if (gameState->GetCharacterCount() == 0)
	{
		friendlyCount = 0;
		enemyCount = 0;
		OnPlayerStateChanged.Broadcast();
		if (gamePlayState == GamePlayState::PLAYING)
			SetGamePlayState(GamePlayState::PLAYINGTOCOMPLETE);
		return;
	}
	
	factionCharacterCount = gameState->GetFactionCharacterCount();

	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwner());
	auto playerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());

	if (!playerCharacter) return;
	

	friendlyCount = factionCharacterCount[playerCharacter->GetCharacterStatData().faction];

	enemyCount = gameState->GetCharacterCount() - friendlyCount;

	OnPlayerStateChanged.Broadcast();

	if (enemyCount == 0)
	{
		if (gamePlayState == GamePlayState::PLAYING)
			SetGamePlayState(GamePlayState::PLAYINGTOCOMPLETE);	
	}
}

void AMainPlayerState::SetGamePlayState(GamePlayState state)
{
	gamePlayState = state;
	OnGamePlayStateChanged.Broadcast();
	OnDynamicGamePlayStateChanged.Broadcast();
}


void AMainPlayerState::InitGamePlayState()
{
	FString jsonStr;

	FString fileName = TEXT("/Save/") + GetWorld()->GetName() + TEXT(".json");
	FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + fileName));


	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	GamePlayState loadGameState;

	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		UE_LOG(LogClass, Warning, TEXT("Deserialize"));

		int state = jsonObject->GetNumberField("GamePlayState");


		switch (state)
		{
		case 0:
			loadGameState = GamePlayState::BEGIN;
			break;
		case 1:
			loadGameState = GamePlayState::BEGINTOPLAYING;
			break;
		case 2:
			loadGameState = GamePlayState::PLAYING;
			break;
		case 3:
			loadGameState = GamePlayState::PLAYINGTOCOMPLETE;
			break;
		case 4:
			loadGameState = GamePlayState::COMPLETE;
			break;
		default:
			break;
		}
	}
	else
		loadGameState = GamePlayState::BEGIN;
	
	SetGamePlayState(loadGameState);
}

int AMainPlayerState::GetGamePlayStateToInt()
{
	int state = 0;
	switch (gamePlayState)
	{
	case BEGIN:
		state = 0;
		break;
	case BEGINTOPLAYING:
		state = 1;
		break;
	case PLAYING:
		state = 2;
		break;
	case PLAYINGTOCOMPLETE:
		state = 3;
		break;
	case COMPLETE:
		state = 4;
		break;
	default:
		break;
	}

	return state;
}

