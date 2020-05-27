// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameSetting.h"
#include "BaseCharacter.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"

// Sets default values
ANewGameSetting::ANewGameSetting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANewGameSetting::BeginPlay()
{
	Super::BeginPlay();
	

	DefaultWriteCharacterDataFile(TEXT("Field"));
	DefaultWriteCharacterDataFile(TEXT("Demonstration"));
	DefaultWriteCharacterDataFile(TEXT("LV_Soul_Cave"));
	DefaultWriteCharacterDataFile(TEXT("Player"));
	DefaultWriteCharacterDataFile(TEXT("Grassland"));

	UGameplayStatics::OpenLevel(this, TEXT("LoadingLevel"));

}

// Called every frame
void ANewGameSetting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANewGameSetting::DefaultWriteCharacterDataFile(FString fileName)
{
	FString jsonStr;

	FString defaultFileName = TEXT("/Save/DefaultSetting/") + fileName + TEXT("DefaultSetting.json");
	FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + defaultFileName));


	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());


	FJsonSerializer::Deserialize(reader, jsonObject);
	   	



	TSharedRef< TJsonWriter<> > writer = TJsonWriterFactory<>::Create(&jsonStr);

	FString writeFileName = TEXT("/Save/") + fileName + TEXT(".json");

	FFileHelper::SaveStringToFile(*jsonStr, *(FPaths::LaunchDir() + writeFileName));
}
