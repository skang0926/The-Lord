// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "HumanCharacter.h"
#include "CharacterSpawner.h"
#include "UserPlayerController.h"
#include "MainGameState.h"
#include "MainPlayerState.h"

AMainGameMode::AMainGameMode()
{	
	PlayerControllerClass = AUserPlayerController::StaticClass();
	GameStateClass = AMainGameState::StaticClass();
	PlayerStateClass = AMainPlayerState::StaticClass();
}

void AMainGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mainGameState = dynamic_cast<AMainGameState*>(GameState);
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMainGameMode::SetCharacterCount(TArray<ABaseCharacter*> characterList)
{	
	if (mainGameState)
	{
		mainGameState->SetCharacterCount(characterList);
	}
}