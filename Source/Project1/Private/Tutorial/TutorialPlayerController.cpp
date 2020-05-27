// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialPlayerController.h"
#include "UserHUDWidget.h"
#include "TutorialCharacter.h"
#include "TutorialWidget.h"
#include "Components/WidgetComponent.h"

ATutorialPlayerController::ATutorialPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserHUDWidget>UI_HUD(TEXT("/Game/UI/UI_PlayerHUD.UI_PlayerHUD_C"));
	if (UI_HUD.Succeeded())
	{	
		HUDWidgetClass = UI_HUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UTutorialWidget>UI_TUTORIALWIDGET(TEXT("/Game/UI/UI_TutorialCommand.UI_TutorialCommand_C"));
	if (UI_TUTORIALWIDGET.Succeeded())
	{	
		tutorialWidgetClass = UI_TUTORIALWIDGET.Class;
	}
}

void ATutorialPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserHUDWidget>(this, HUDWidgetClass);


	auto player = dynamic_cast<ATutorialCharacter*>(GetPawn());
	if (player)
	{
		HUDWidget->BindCharacterWidget(player->characterStat);
	}

	tutorialWidget = CreateWidget<UTutorialWidget>(this, tutorialWidgetClass);

	tutorialWidget->BindTutorialWidget(this);

	tutorialWidget->AddToViewport(0);

//	tutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	
}

void ATutorialPlayerController::ChangeInputMode(bool bInputMode)
{
	if (bInputMode)
	{
		SetInputMode(gameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ATutorialPlayerController::SetChangeTutorialState(TutorialState state)
{
	if (state == TutorialState::SkillATutorial)
	{
		HUDWidget->AddToViewport();
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	tutorialState = state;
	OnTutorialChange.Broadcast();
	OnTutorialStateChange.Broadcast();
}

TutorialState ATutorialPlayerController::GetTutorialState()
{	
	return tutorialState;
}