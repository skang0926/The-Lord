// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanCharacterWidget.h"
#include "BaseCharacter.h"
#include "CharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UHumanCharacterWidget::BindCharacterWidget(UCharacterStatComponent* characterStat)
{
	currentCharacterStat = characterStat;	
	currentCharacterStat->OnHpChange.AddUObject(this, &UHumanCharacterWidget::UpdateHPWidget);
}

void UHumanCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	hpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));	
	UpdateHPWidget();
}

void UHumanCharacterWidget::UpdateHPWidget()
{
	if (currentCharacterStat)
	{
		if (hpProgressBar != nullptr)
		{
			hpProgressBar->SetPercent(currentCharacterStat->GetHpRatio());
		}
	}	
}