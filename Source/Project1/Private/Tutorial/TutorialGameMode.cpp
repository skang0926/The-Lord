// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialGameMode.h"
#include "TutorialCharacter.h"
#include "TutorialPlayerController.h"
#include "CharacterSpawner.h"
#include "MainGameState.h"
#include "MainPlayerState.h"


ATutorialGameMode::ATutorialGameMode()
{
	PlayerControllerClass = ATutorialPlayerController::StaticClass();
	DefaultPawnClass = ATutorialCharacter::StaticClass();
}

void ATutorialGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATutorialGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
