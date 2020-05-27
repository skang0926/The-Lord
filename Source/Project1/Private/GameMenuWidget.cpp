// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuWidget.h"
#include "Components/Button.h"
#include "UserPlayerController.h"


void UGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	resumeButton = Cast<UButton>(GetWidgetFromName(TEXT("B_Resume")));
	if (nullptr != resumeButton)
	{
		resumeButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnResumeClicked);
	}


	saveButton = Cast<UButton>(GetWidgetFromName(TEXT("B_Save")));
	if (nullptr != saveButton)
	{
		saveButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnSaveClicked);
	}

	loadButton = Cast<UButton>(GetWidgetFromName(TEXT("B_Load")));
	if (nullptr != loadButton)
	{
		loadButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnLoadClicked);
	}
	

	exitButton = Cast<UButton>(GetWidgetFromName(TEXT("B_Exit")));
	if (nullptr != exitButton)
	{
		exitButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnExitClicked);
	}
}

void UGameMenuWidget::OnResumeClicked()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());

//	RemoveFromViewport();
	SetVisibility(ESlateVisibility::Hidden);

	playerController->ChangeInputMode(true);
	playerController->SetPause(false);
}

void UGameMenuWidget::OnSaveClicked()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());
//	RemoveFromViewport();
	SetVisibility(ESlateVisibility::Hidden);
	playerController->ChangeInputMode(true);
	playerController->SetPause(false);
	playerController->SaveGame();
}

void UGameMenuWidget::OnLoadClicked()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());
	playerController->ChangeInputMode(true);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingLevel"));
}

void UGameMenuWidget::OnExitClicked()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());
	playerController->ChangeInputMode(true);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}


