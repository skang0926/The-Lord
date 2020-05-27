// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawner.h"
#include "BaseCharacter.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "CharacterStatComponent.h"
#include "UserPlayerController.h"
#include "MainGameState.h"
#include "MainGameMode.h"
#include "MainPlayerState.h"
#include "CharacterDataTable.h"
// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}



// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();

	ReadPlayerDataFile();
	if (playerCharacter)
	{
		auto controller = dynamic_cast<AUserPlayerController*>(playerCharacter->GetController());
		if (controller)
		{
			controller->OnSaveGame.AddUObject(this, &ACharacterSpawner::SaveGame);
		}
	}

	switch (readWriteSetting)
	{
	case READONLY:
	case READANDWRITE:
		ReadCharacterDataFile();
		break;
	default:
		break;
	}
}

void ACharacterSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (readWriteSetting)
	{
	case WRITEONLY:
	case READANDWRITE:
		WriteCharacterDataFile();
		break;
	default:
		break;
	}
}


// Called every frame
void ACharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACharacterSpawner::WriteCharacterDataFile()
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	FString outputString;

	TSharedRef< TJsonWriter<> > writer = TJsonWriterFactory<>::Create(&outputString);

	FString fileName = TEXT("/Save/") + GetWorld()->GetName() + TEXT(".json");
	
	TArray<TSharedPtr<FJsonValue>> jsonObjectArr;	

	for (auto actor : GetLevel()->Actors)
	{
		AHumanCharacter* character = dynamic_cast<AHumanCharacter*>(actor);
		if (character)
		{			
			if ((character->characterName.ToString() == "Player") || !(character->GetWritePermit()))
				continue;

			TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
			TSharedPtr<FJsonValue> jsonObjValue = MakeShareable(new FJsonValueObject(jsonObj));
			
			FString name = character->characterName.ToString();
			FVector characterLocation = character->GetActorLocation();
			FRotator characterRotator = character->GetActorRotation();
			jsonObj->SetStringField("Name", name);
			jsonObj->SetNumberField("Location X", characterLocation.X);
			jsonObj->SetNumberField("Location Y", characterLocation.Y);

			jsonObj->SetNumberField("Location Z", characterLocation.Z);
			if (character->characterStat->GetCurrentHp() <= 0.f)
				jsonObj->SetNumberField("Location Z", characterLocation.Z + 160);

			jsonObj->SetNumberField("Rotator Pitch", characterRotator.Pitch);
			jsonObj->SetNumberField("Rotator Yaw", characterRotator.Yaw);
			jsonObj->SetNumberField("Rotator Roll", characterRotator.Roll);
			jsonObj->SetNumberField("CurrentHP", character->characterStat->GetCurrentHp());
			jsonObjectArr.Add(jsonObjValue);
			
		}
	}

	jsonObject->SetArrayField("CharacterList", jsonObjectArr);
	

	int32 state = 0;
	if (playerCharacter)
	{
		auto playerState = dynamic_cast<AMainPlayerState*>(playerCharacter->GetPlayerState());
		state = playerState->GetGamePlayStateToInt();	
	}
	jsonObject->SetNumberField("GamePlayState", state);

	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	FFileHelper::SaveStringToFile(*outputString, *(FPaths::LaunchDir() + fileName));
}

void ACharacterSpawner::ReadCharacterDataFile()
{
	FString jsonStr;	
	
	FString fileName = TEXT("/Save/") + GetWorld()->GetName() + TEXT(".json");
	FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + fileName));	
	

	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());


	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		UE_LOG(LogClass, Warning, TEXT("Deserialize"));
		TArray<TSharedPtr<FJsonValue>> jsonObjectArr = jsonObject->GetArrayField(TEXT("CharacterList"));
		TArray<ABaseCharacter*> characterList;
		for (auto jsonObject : jsonObjectArr)
		{			
			FString name;
			FVector location;
			FRotator rotator;
			name = jsonObject->AsObject()->GetStringField("Name");
			location.X = jsonObject->AsObject()->GetNumberField("Location X");
			location.Y = jsonObject->AsObject()->GetNumberField("Location Y");
			location.Z = jsonObject->AsObject()->GetNumberField("Location Z");
			rotator.Pitch = jsonObject->AsObject()->GetNumberField("Rotator Pitch");
			rotator.Yaw = jsonObject->AsObject()->GetNumberField("Rotator Yaw");
			rotator.Roll = jsonObject->AsObject()->GetNumberField("Rotator Roll");
			float hp = jsonObject->AsObject()->GetNumberField("CurrentHP");

			auto character = GetWorld()->SpawnActor<AHumanCharacter>(AHumanCharacter::StaticClass(), location, rotator);
			character->InitCharacter(FName(*name), hp);		

			characterList.Add(character);		
		}

		auto gameMode = dynamic_cast<AMainGameMode*>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			if (characterList.IsValidIndex(0))
			{
				gameMode->SetCharacterCount(characterList);
			}		
		}		
		
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Load fail"));
	}

	
}

void ACharacterSpawner::ReadPlayerDataFile()
{
	FString jsonStr;

	FString fileName = TEXT("/Save/Player.json");
	
	FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + fileName));



	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());


	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		FString name;
		FVector location;
		FRotator rotator;
		name = jsonObject->GetStringField("Name");

		FString world = jsonObject->GetStringField("World");
		if (GetWorld()->GetName() != world)	 return;

		location.X = jsonObject->GetNumberField("Location X");
		location.Y = jsonObject->GetNumberField("Location Y");
		location.Z = jsonObject->GetNumberField("Location Z");
		rotator.Pitch = jsonObject->GetNumberField("Rotator Pitch");
		rotator.Yaw = jsonObject->GetNumberField("Rotator Yaw");
		rotator.Roll = jsonObject->GetNumberField("Rotator Roll");
		float hp = jsonObject->GetNumberField("CurrentHP");

		auto character = GetWorld()->SpawnActor<AHumanCharacter>(AHumanCharacter::StaticClass(), location, rotator);
		character->InitCharacter(FName(*name), hp);		

		playerCharacter = character;		
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Load fail"));
	}
}

void ACharacterSpawner::WritePlayerDataFile(AActor* otherActor)
{
	AHumanCharacter* player = dynamic_cast<AHumanCharacter*>(otherActor);
	if (!player) return;


	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	FString outputString;

	TSharedRef< TJsonWriter<> > writer = TJsonWriterFactory<>::Create(&outputString);

	FString fileName = TEXT("/Save/Player.json");



	FString name = player->characterName.ToString();
	FVector location = player->GetActorLocation();
	FRotator rotator = player->GetActorRotation();

	jsonObject->SetStringField("Name", name);
	jsonObject->SetStringField("World", GetWorld()->GetName());
	jsonObject->SetNumberField("Location X", location.X);
	jsonObject->SetNumberField("Location Y", location.Y);
	jsonObject->SetNumberField("Location Z", location.Z);
	jsonObject->SetNumberField("Rotator Pitch", rotator.Pitch);
	jsonObject->SetNumberField("Rotator Yaw", rotator.Yaw);
	jsonObject->SetNumberField("Rotator Roll", rotator.Roll);
	jsonObject->SetNumberField("CurrentHP", player->characterStat->GetCurrentHp());


	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	FFileHelper::SaveStringToFile(*outputString, *(FPaths::LaunchDir() + fileName));
}

void ACharacterSpawner::SaveGame()
{
	WritePlayerDataFile(playerCharacter);
	WriteCharacterDataFile();
}