// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()	

public:
	void BindCharacterWidget(class UCharacterStatComponent*);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateStaminaWidget();
private:
	class UCharacterStatComponent* currentCharacterStat;



	UPROPERTY()
		class UProgressBar* hpProgressBar;

	UPROPERTY()
		class UProgressBar* staminaProgressBar;
};
