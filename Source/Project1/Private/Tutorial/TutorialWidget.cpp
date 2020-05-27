// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "Components/Image.h"
#include "TutorialPlayerController.h"

void UTutorialWidget::BindTutorialWidget(ATutorialPlayerController* playerController)
{
	this->playerController = playerController;

	playerController->OnTutorialStateChange.AddUObject(this, &UTutorialWidget::UpdateTutorialWidget);
}



void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	imgNum = 14;
	index = 1;

	for (int i = 0; i < imgNum; i++)
	{
		FName tutorialLevel = *FString::Printf(TEXT("%d"), i);
		UImage* imgName = Cast<UImage>(GetWidgetFromName(tutorialLevel));
		imgName->SetVisibility(ESlateVisibility::Hidden);
		imgTutorial.Add(imgName);
	}
}

void UTutorialWidget::UpdateTutorialWidget()
{
	for (auto img : imgTutorial)
		img->SetVisibility(ESlateVisibility::Hidden);
	

	if (index < 14)
		imgTutorial[index++]->SetVisibility(ESlateVisibility::Visible);

}

