// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindTutorialWidget(class ATutorialPlayerController*);

protected:
	virtual void NativeConstruct() override;
	void UpdateTutorialWidget();

private:
	class ATutorialPlayerController* playerController;

	int32 index;
	int32 imgNum;

	UPROPERTY()
		TArray<class UImage*> imgTutorial;	
};
