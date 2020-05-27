// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterGameInstance.h"

UCharacterGameInstance::UCharacterGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_CHARACTER(TEXT("/Game/Characters/DT_CharacterDataTable.DT_CharacterDataTable"));
	if (DT_CHARACTER.Succeeded())
	{
		characterDataTable = DT_CHARACTER.Object;
	}
}

void UCharacterGameInstance::Init()
{
	
}

FCharacterDataTable* UCharacterGameInstance::GetCharacterData(const FName& characterName)
{
	return characterDataTable->FindRow<FCharacterDataTable>(characterName, FString(), false);
}