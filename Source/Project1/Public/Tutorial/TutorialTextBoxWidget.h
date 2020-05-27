// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Blueprint/UserWidget.h"
#include "TutorialTextBoxWidget.generated.h"

/**
 * 
 */

struct TutorialDialogue
{
	int imgType;
	FString text;
};

UCLASS()
class PROJECT1_API UTutorialTextBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	void ReadJsonFile();

	TWeakObjectPtr<class AMainPlayerState> currentPlayerState;

	UFUNCTION()
		void OnButtonClicked();

	UPROPERTY()
		class UTextBlock* textBox;

	UPROPERTY()
		class UButton* button;

	UPROPERTY()
		TArray<class UImage*> dialougeCharacterImg;
	int32 textIndex;
	int32 textCount;

	TArray<TutorialDialogue> dialougeArr;
};
