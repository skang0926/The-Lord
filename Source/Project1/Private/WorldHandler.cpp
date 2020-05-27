// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldHandler.h"
#include "BaseCharacter.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "CharacterStatComponent.h"

// Sets default values
AWorldHandler::AWorldHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWorldHandler::BeginPlay()
{
	Super::BeginPlay();
	

	ReadPlayerDataFile();
}

// Called every frame
void AWorldHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldHandler::ReadPlayerDataFile()
{
	FString jsonStr;

	FString fileName = TEXT("/Save/Player.json");
	if (FFileHelper::LoadFileToString(jsonStr, *(FPaths::LaunchDir() + fileName)))
	{
		UE_LOG(LogClass, Warning, TEXT("%s"), *jsonStr);
	}
	else
	{
		UGameplayStatics::OpenLevel(this, TEXT("Title"));
	}



	TSharedRef< TJsonReader<TCHAR> > reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());


	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		FString world = jsonObject->GetStringField("World");

		UGameplayStatics::OpenLevel(this, FName(*world));
	}

}

