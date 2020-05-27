// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBoxWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UserPlayerController.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "MainPlayerState.h"

void UTextBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	button = Cast<UButton>(GetWidgetFromName(TEXT("B_NextText")));
	if (nullptr != button)
	{
		button->OnClicked.AddDynamic(this, &UTextBoxWidget::OnButtonClicked);
	}

	textBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("MainTextBox")));

	auto playerState = dynamic_cast<AMainPlayerState*>(GetOwningPlayerState());
	if (playerState)
	{
		currentPlayerState = playerState;		
	}


	for (int i = 0; i < 5; i++)
	{
		FName characterType = *FString::Printf(TEXT("%d"), i);
		UImage* imgName = Cast<UImage>(GetWidgetFromName(characterType));
		dialougeCharacterImg.Add(imgName);
	}

	ReadJsonFile();

	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());

	playerController->ChangeInputMode(false);


	auto playerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());
	if (!playerCharacter) return;

	playerCharacter->SetCanMove(false);


	textIndex = 0;
	OnButtonClicked();	
}



void UTextBoxWidget::OnButtonClicked()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetOwningPlayer());

	playerController->ChangeInputMode(false);


	auto playerCharacter = dynamic_cast<ABaseCharacter*>(playerController->GetPawn());
	if (!playerCharacter) return;

	playerCharacter->SetCanMove(false);

	if (textCount-1 < textIndex)
	{
		playerController->ChangeInputMode(true);
		playerController->SetPause(false);
		RemoveFromParent();

		playerCharacter->SetCanMove(true);

		return;
	}

	int characterType = dialougeArr[textIndex].characterType;
	FString text = dialougeArr[textIndex++].text;

	for (auto img : dialougeCharacterImg)	
		img->SetVisibility(ESlateVisibility::Hidden);	

	dialougeCharacterImg[characterType]->SetVisibility(ESlateVisibility::Visible);



	
	textBox->SetText(FText::FromString(text));
}

void UTextBoxWidget::ReadJsonFile()
{
	FString gamePlayState;
	switch (currentPlayerState->GetGamePlayStateToInt())	
	{
	case 0:
		gamePlayState = FString(TEXT("Begin"));
		break;
	case 1:
		gamePlayState = FString(TEXT("BeginToPlay"));
		break;	
	case 3:
		gamePlayState = FString(TEXT("PlayingToComplete"));
		break;
	default:
		gamePlayState = nullptr;
		break;

	}

	FString jsonStr;

	FString fileName = TEXT("/Save/DefaultSetting/Text/") + GetWorld()->GetName() + gamePlayState + TEXT("TextFile.json");

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
			Dialogue dialouge;
			dialouge.characterType = jsonObject->AsObject()->GetNumberField("CharacterType");;
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