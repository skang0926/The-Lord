// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATutorialGameMode();
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
