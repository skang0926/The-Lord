// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialTextBoxWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TutorialPlayerController.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "MainPlayerState.h"

void UTutorialTextBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	button = Cast<UButton>(GetWidgetFromName(TEXT("B_NextText")));
	if (nullptr != button)
	{
		button->OnClicked.AddDynamic(this, &UTutorialTextBoxWidget::OnButtonClicked);
	}

	textBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("MainTextBox")));




	for (int i = 0; i < 1; i++)
	{
		FName characterType = *FString::Printf(TEXT("%d"), i);
		UImage* imgName = Cast<UImage>(GetWidgetFromName(characterType));
		dialougeCharacterImg.Add(imgName);
	}

	ReadJsonFile();

	auto playerController = dynamic_cast<ATutorialPlayerController*>(GetOwningPlayer());
	if (!playerController) return;


	auto playerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());
	if (!playerCharacter) return;

	playerCharacter->SetCanMove(false);


	textIndex = 0;
	OnButtonClicked();
}



void UTutorialTextBoxWidget::OnButtonClicked()
{
	auto playerController = dynamic_cast<ATutorialPlayerController*>(GetOwningPlayer());

	playerController->ChangeInputMode(false);


	auto playerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());
	if (!playerCharacter) return;

	playerCharacter->SetCanMove(false);

	if (textCount - 1 < textIndex)
	{
		if (playerController->GetTutorialState() == TutorialState::CompleteTutorial)
		{
			UGameplayStatics::OpenLevel(this, TEXT("NewGame"));
		}

		playerController->ChangeInputMode(true);
		playerController->SetPause(false);
		RemoveFromParent();

		playerCharacter->SetCanMove(true);

		return;
	}

	int imgType = dialougeArr[textIndex].imgType;
	FString text = dialougeArr[textIndex++].text;

	for (auto img : dialougeCharacterImg)
		img->SetVisibility(ESlateVisibility::Hidden);

	dialougeCharacterImg[imgType]->SetVisibility(ESlateVisibility::Visible);




	textBox->SetText(FText::FromString(text));
}

void UTutorialTextBoxWidget::ReadJsonFile()
{
	auto playerController = dynamic_cast<ATutorialPlayerController*>(GetOwningPlayer());

	FString state;
	switch(playerController->tutorialState)
	{
	case TutorialState::MoveTutorial:
		state = TEXT("MoveTutorial");
		break;
	case TutorialState::EquipTutorial:
		state = TEXT("EquipTutorial");
		break;
	case TutorialState::LockOnTutorial:
		state = TEXT("LockOnTutorial");
		break;
	case TutorialState::InPlaceAttackTutorial:
		state = TEXT("InPlaceAttackTutorial");
		break;	
	case TutorialState::ForwardAttackTutorial:
		state = TEXT("ForwardAttackTutorial");
		break;
	case TutorialState::PierceAttackTutorial:
		state = TEXT("PierceAttackTutorial");
		break;
	case TutorialState::SkillATutorial:
		state = TEXT("SkillATutorial");
		break;
	case TutorialState::SkillBTutorial:
		state = TEXT("SkillBTutorial");
		break;
	case TutorialState::SkillModeTutorial:
		state = TEXT("SkillModeTutorial");
		break;
	case TutorialState::SkillAttackTutorial:
		state = TEXT("SkillAttackTutorial");
		break;
	case TutorialState::MoveToGuardTutorial:
		state = TEXT("MoveToGuardTutorial");
		break;
	case TutorialState::GuardTutorial:
		state = TEXT("GuardTutorial");		
		break;
	case TutorialState::ParryTutorial:
		state = TEXT("ParryTutorial");
		playerController->SetPause(true);
		break;
	case TutorialState::ParryCountTutorial:
		state = TEXT("ParryCountTutorial");
		playerController->SetPause(true);
		break;
	case TutorialState::CompleteTutorial:
		state = TEXT("CompleteTutorial");
		playerController->SetPause(true);
		break;
	default:
		break;
	}

	FString jsonStr;

	FString fileName = TEXT("/Save/DefaultSetting/Text/Tutorial/") + state + TEXT("TextFile.json");

	FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + fileName));


	UE_LOG(LogClass, Warning, TEXT("%s"), *fileName);
	UE_LOG(LogClass, Warning, TEXT("%s"), *jsonStr);


	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		UE_LOG(LogClass, Warning, TEXT("Deserialize"));
		TArray<TSharedPtr<FJsonValue>> jsonObjectArr = jsonObject->GetArrayField(TEXT("Dialogue"));

		for (auto jsonObject : jsonObjectArr)
		{
			TutorialDialogue dialouge;
			dialouge.imgType = jsonObject->AsObject()->GetNumberField("imgType");;
			dialouge.text = jsonObject->AsObject()->GetStringField("Text");
			dialougeArr.Add(dialouge);
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Json Deserialize Fail"));
		return;
	}

	textCount = dialougeArr.Num();
}