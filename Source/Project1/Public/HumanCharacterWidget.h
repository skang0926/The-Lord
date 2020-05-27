// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Blueprint/UserWidget.h"
#include "HumanCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UHumanCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void BindCharacterWidget(class UCharacterStatComponent* newHumanCharacter);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	class UCharacterStatComponent* currentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* hpProgressBar;
};
