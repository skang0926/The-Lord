// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidget.h"
#include "CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UBossWidget::BindCharacterWidget(UCharacterStatComponent* characterStat)
{
	currentCharacterStat = characterStat;
	currentCharacterStat->OnHpChange.AddUObject(this, &UBossWidget::UpdateHPWidget);
	currentCharacterStat->OnStaminaChanged.AddUObject(this, &UBossWidget::UpdateStaminaWidget);
	UpdateHPWidget();
	UpdateStaminaWidget();
}

void UBossWidget::NativeConstruct()
{
	Super::NativeConstruct();


	hpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	UpdateHPWidget();

	staminaProgressBar = Cast< UProgressBar>(GetWidgetFromName(TEXT("PB_StaminaBar")));
	UpdateStaminaWidget();
}

void UBossWidget::UpdateHPWidget()
{
	if (currentCharacterStat)
	{
		if (hpProgressBar != nullptr)
		{
			hpProgressBar->SetPercent(currentCharacterStat->GetHpRatio());
		}
	}
}

void UBossWidget::UpdateStaminaWidget()
{
	if (currentCharacterStat)
	{
		if (staminaProgressBar != nullptr)
		{
			staminaProgressBar->SetPercent(currentCharacterStat->GetStaminaRatio());
		}
	}
}

