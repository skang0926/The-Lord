// Fill out your copyright notice in the Description page of Project Settings.


#include "UserPlayerController.h"
#include "UserHUDWidget.h"
#include "Components/WidgetComponent.h"
#include "HumanCharacter.h"
#include "GameMenuWidget.h"
#include "BossWidget.h"
#include "MainGameMode.h"
#include "CharacterStatComponent.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "CharacterStatComponent.h"
#include "MainPlayerState.h"

AUserPlayerController::AUserPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserHUDWidget>UI_HUD(TEXT("/Game/UI/UI_PlayerHUD.UI_PlayerHUD_C"));
	if (UI_HUD.Succeeded())
	{
		UE_LOG(LogClass, Warning, TEXT("HUD Widget iNIT ! "));
		HUDWidgetClass = UI_HUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameMenuWidget>UI_MENU_C(TEXT("/Game/UI/UI_GameMenu.UI_GameMenu_C"));
	if (UI_MENU_C.Succeeded())
	{
		menuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UBossWidget>UI_BOSSHUD_C(TEXT("/Game/UI/UI_BossHUDWidget.UI_BossHUDWidget_C"));
	if (UI_BOSSHUD_C.Succeeded())
	{
		bossHUDWidgetClass = UI_BOSSHUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>UI_DEADHUD_C(TEXT("/Game/UI/UI_DEAD.UI_Dead_C"));
	if (UI_DEADHUD_C.Succeeded())
	{
		deadWidgetClass = UI_DEADHUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>UI_ENDINGHUD_C(TEXT("/Game/UI/UI_CompleteGame.UI_CompleteGame_C"));
	if (UI_ENDINGHUD_C.Succeeded())
	{
		completeGameWidgetClass = UI_ENDINGHUD_C.Class;
	}
}

void AUserPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	auto player = dynamic_cast<AHumanCharacter*>(GetPawn());
	if (player)
	{
		HUDWidget->BindCharacterWidget(player->characterStat);
	}


	bossHUDWidget = CreateWidget<UBossWidget>(this, bossHUDWidgetClass);
	bossHUDWidget->AddToViewport();
	bossHUDWidget->SetVisibility(ESlateVisibility::Hidden);


	menuWidget = CreateWidget<UGameMenuWidget>(this, menuWidgetClass);

	mainPlayerState = dynamic_cast<AMainPlayerState*>(PlayerState);

	HUDWidget->BindPlayerState(mainPlayerState);

	menuWidget->SetVisibility(ESlateVisibility::Hidden);
	menuWidget->AddToViewport(3);


	deadWidget = CreateWidget<UUserWidget>(this, deadWidgetClass);
	deadWidget->SetVisibility(ESlateVisibility::Hidden);

	completeGameWidget = CreateWidget<UUserWidget>(this, completeGameWidgetClass);
	completeGameWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AUserPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AUserPlayerController::OnGamePause);
}

void AUserPlayerController::AddDeadWidget()
{
	deadWidget->AddToViewport(5);
	deadWidget->SetVisibility(ESlateVisibility::Visible);

	ChangeInputMode(false);

	GetWorldTimerManager().SetTimer(gamePauseTimerHandle, this, &AUserPlayerController::Pause, 0.1f, false, 5.f);
}

void AUserPlayerController::AddGameCompleteWidget()
{
	completeGameWidget->AddToViewport(5);
	completeGameWidget->SetVisibility(ESlateVisibility::Visible);

	ChangeInputMode(false);
}

void AUserPlayerController::OnGamePause()
{	
	menuWidget->SetVisibility(ESlateVisibility::Visible);

	SetPause(true);
	ChangeInputMode(false);
}

void AUserPlayerController::ChangeInputMode(bool bInputMode)
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


void AUserPlayerController::SaveGame()
{
	OnSaveGame.Broadcast();
}

void AUserPlayerController::SetHUDVisible(bool bFlag)
{
	if (!HUDWidget)
	{
		UE_LOG(LogClass, Warning, TEXT("HudWidgetNone"));
		return;
	}

	if (bFlag)
	{
		HUDWidget->AddToViewport(0);
	}
	else
	{
		HUDWidget->RemoveFromViewport();
	}
}

void AUserPlayerController::Pause()
{
	SetPause(true);
}

void AUserPlayerController::SetEnemyHUD(UCharacterStatComponent* enemyCharacterStat)
{
	bossHUDWidget->SetVisibility(ESlateVisibility::Visible);
	bossHUDWidget->BindCharacterWidget(enemyCharacterStat);
}

UUserHUDWidget* AUserPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}
