// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"

#include "GameFramework/PlayerController.h"
#include "UserPlayerController.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnSaveGameDelegate)

UCLASS(BlueprintType, Blueprintable)
class PROJECT1_API AUserPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
public:
	AUserPlayerController();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UBossWidget> bossHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UGameMenuWidget>menuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> deadWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> completeGameWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UUserHUDWidget* HUDWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UBossWidget* bossHUDWidget;

	UPROPERTY()
		class UGameMenuWidget* menuWidget;

	UPROPERTY()
		class UUserWidget* deadWidget;

	UPROPERTY()
		class UUserWidget* completeGameWidget;

	

	FInputModeGameOnly gameInputMode;
	FInputModeUIOnly UIInputMode;

	FOnSaveGameDelegate OnSaveGame;


	void SaveGame();
	void OnGamePause();
	void ChangeInputMode(bool bInputMode = true);

	void AddDeadWidget();
	void AddGameCompleteWidget();

	void Pause();
	UUserHUDWidget* GetHUDWidget() const;

	class AMainPlayerState* GetPlayerState() { return mainPlayerState; }

	
	void SetEnemyHUD(class UCharacterStatComponent*);

	UFUNCTION(BlueprintCallable, Category = SetUI)
		void SetHUDVisible(bool bFlag);

	FTimerHandle gamePauseTimerHandle = {};

private:
	UPROPERTY()
		class AMainPlayerState* mainPlayerState;
};
