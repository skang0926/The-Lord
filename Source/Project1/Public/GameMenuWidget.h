// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnResumeClicked();

	UFUNCTION()
		void OnSaveClicked();

	UFUNCTION()
		void OnLoadClicked();

	UFUNCTION()
		void OnExitClicked();

protected:
	UPROPERTY()
		class UButton* resumeButton;

	UPROPERTY()
		class UButton* saveButton;

	UPROPERTY()
		class UButton* loadButton;

	UPROPERTY()
		class UButton* exitButton;

};
