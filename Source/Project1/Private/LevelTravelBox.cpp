// Fill out your copyright notice in the Description page of Project Settings.

	
#include "LevelTravelBox.h"
#include "BaseCharacter.h"
#include "HumanCharacter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "CharacterStatComponent.h"
#include "UserPlayerController.h"

ALevelTravelBox::ALevelTravelBox()
{
	
}


void ALevelTravelBox::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelTravelBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALevelTravelBox::OnBoxOverlap);


}

void ALevelTravelBox::OnBoxOverlap(UPrimitiveComponent* overlapComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = dynamic_cast<AHumanCharacter*>(otherActor);
	if (!player) return;
	
	if (player->GetController()->IsPlayerController())
	{	
		auto playerController = dynamic_cast<AUserPlayerController*>(player->GetController());
		playerController->SaveGame();
		WritePlayerDataFile(otherActor);		
	
		UGameplayStatics::OpenLevel(this, TEXT("LoadingLevel"));
	}
}

void ALevelTravelBox::WritePlayerDataFile(AActor* otherActor)
{
	AHumanCharacter* player = dynamic_cast<AHumanCharacter*>(otherActor);
	if (!player) return;


	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	FString outputString;

	TSharedRef< TJsonWriter<> > writer = TJsonWriterFactory<>::Create(&outputString);

	FString fileName = TEXT("/Save/Player.json");
	

	
	FString name = player->characterName.ToString();

	jsonObject->SetStringField("Name", name);
	jsonObject->SetStringField("World", nextLevelName.ToString());
	jsonObject->SetNumberField("Location X", nextLevelLocation.X);
	jsonObject->SetNumberField("Location Y", nextLevelLocation.Y);
	jsonObject->SetNumberField("Location Z", nextLevelLocation.Z);
	jsonObject->SetNumberField("Rotator Pitch", nextLevelRotator.Pitch);
	jsonObject->SetNumberField("Rotator Yaw", nextLevelRotator.Yaw);
	jsonObject->SetNumberField("Rotator Roll", nextLevelRotator.Roll);
	jsonObject->SetNumberField("CurrentHP", player->characterStat->GetCurrentHp());


	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	FFileHelper::SaveStringToFile(*outputString, *(FPaths::LaunchDir() + fileName));
}