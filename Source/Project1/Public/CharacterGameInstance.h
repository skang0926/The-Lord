// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "CharacterDataTable.h"
#include "Engine/StreamableManager.h"
#include "CharacterGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT1_API UCharacterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCharacterGameInstance();
	
	virtual void Init() override;
	FCharacterDataTable* GetCharacterData(const FName& characterName);
		
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
		class UDataTable* characterDataTable;		
};
