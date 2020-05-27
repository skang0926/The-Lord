// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Blueprint/UserWidget.h"
#include "UserHUDWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECT1_API UUserHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterWidget(class UCharacterStatComponent*);
	void BindPlayerState(class AMainPlayerState* PlayerState);
	


	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* hpBarMaterial = nullptr;

	UMaterialInstanceDynamic* hpMaterialInstance = nullptr;

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateStaminaWidget();
	void UpdatePlayerState();
	void UpdateGamePlayState();
private:
	class UCharacterStatComponent* currentCharacterStat;
	TWeakObjectPtr<class AMainPlayerState> currentPlayerState;


	UPROPERTY()
		class UProgressBar* hpProgressBar;

	UPROPERTY()
		class UProgressBar* staminaProgressBar;
	UPROPERTY()
		class UTextBlock* remainEnemyCount;

	UPROPERTY()
		class UTextBlock* enemyCount;
};
