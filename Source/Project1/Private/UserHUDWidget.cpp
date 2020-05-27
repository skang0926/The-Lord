// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHUDWidget.h"
#include "BaseCharacter.h"
#include "CharacterStatComponent.h"
#include "CharacterDataTable.h"
#include "MainPlayerState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UserPlayerController.h"


void UUserHUDWidget::BindCharacterWidget(UCharacterStatComponent* characterStat)
{
	currentCharacterStat = characterStat;
	currentCharacterStat->OnHpChange.AddUObject(this, &UUserHUDWidget::UpdateHPWidget);
	currentCharacterStat->OnStaminaChanged.AddUObject(this, &UUserHUDWidget::UpdateStaminaWidget);
	UpdateHPWidget();
	UpdateStaminaWidget();
}


void UUserHUDWidget::BindPlayerState(class AMainPlayerState* playerState)
{
	currentPlayerState = playerState;
	playerState->OnPlayerStateChanged.AddUObject(this, &UUserHUDWidget::UpdatePlayerState);
	playerState->OnGamePlayStateChanged.AddUObject(this, &UUserHUDWidget::UpdateGamePlayState);
	UpdatePlayerState();
	UpdateGamePlayState();	
}



void UUserHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

	hpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	UpdateHPWidget();

	staminaProgressBar = Cast< UProgressBar>(GetWidgetFromName(TEXT("PB_StaminaBar")));
	UpdateStaminaWidget();


	remainEnemyCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtRemainCount")));

	enemyCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtEnemyCount")));
}

void UUserHUDWidget::UpdateHPWidget()
{
	if (currentCharacterStat)
	{
		if (hpProgressBar != nullptr)
		{
			hpProgressBar->SetPercent(currentCharacterStat->GetHpRatio());
		}
	}
}

void UUserHUDWidget::UpdateStaminaWidget()
{	
	if (currentCharacterStat)
	{
		if (staminaProgressBar != nullptr)
		{
			staminaProgressBar->SetPercent(currentCharacterStat->GetStaminaRatio());
		}
	}
}

void UUserHUDWidget::UpdatePlayerState()
{
	remainEnemyCount->SetVisibility(ESlateVisibility::Hidden);
	enemyCount->SetVisibility(ESlateVisibility::Hidden);	

	if (!(currentPlayerState->IsValidLowLevel())) return;

	switch (currentPlayerState->GetGamePlayState())
	{
	case GamePlayState::PLAYING:
	//	remainEnemyCount->SetVisibility(ESlateVisibility::Visible);
	//	enemyCount->SetVisibility(ESlateVisibility::Visible);
	//	enemyCount->SetText(FText::FromString(FString::FromInt(currentPlayerState->GetEnemyCount())));
		break;
	default:
		break;
	}	
}

void UUserHUDWidget::UpdateGamePlayState()
{
	SetVisibility(ESlateVisibility::Visible);
	remainEnemyCount->SetVisibility(ESlateVisibility::Hidden);
	enemyCount->SetVisibility(ESlateVisibility::Hidden);


	if (!(currentPlayerState->IsValidLowLevel())) return;	

	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());

	switch (currentPlayerState->GetGamePlayState())
	{
	case GamePlayState::BEGIN:
		break;
	case GamePlayState::BEGINTOPLAYING:
	case GamePlayState::PLAYINGTOCOMPLETE:
		playerController->ChangeInputMode(false);		
		SetVisibility(ESlateVisibility::Hidden);
		break;
	case GamePlayState::PLAYING:
	//	remainEnemyCount->SetVisibility(ESlateVisibility::Visible);
	//	enemyCount->SetVisibility(ESlateVisibility::Visible);
	//	enemyCount->SetText(FText::FromString(FString::FromInt(currentPlayerState->GetEnemyCount())));
		break;
	case GamePlayState::COMPLETE:
		break;
	}
}