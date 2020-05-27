// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "BaseCharacter.h"
#include "HumanCharacter.h"
#include "UserPlayerController.h"
#include "CharacterDataTable.h"
#include "MainPlayerState.h"
AMainGameState::AMainGameState()
{
	characterCount = 0;
}

void AMainGameState::SetCharacterCount(TArray<ABaseCharacter*> characterList)
{
	characterCount = characterList.Num();	


	int32 maxFactionIndex = -1;

	for (auto character : characterList)
	{
		int32 factionType = character->GetCharacterStatData().faction;
		if (maxFactionIndex < factionType)
			maxFactionIndex = factionType;
	}

	for (int i = 0; i < maxFactionIndex + 1; i++)
	{
		factionCharacterCount.Add(0);
	}

	if (maxFactionIndex < 0) return;

	for (auto character: characterList)
	{
		int32 factionType = character->GetCharacterStatData().faction;

		factionCharacterCount[factionType]++;
	}
}

void AMainGameState::DecreaseFactionCharacterCount(int32 factionType)
{
	factionCharacterCount[factionType]--;
	characterCount--;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto playerController = dynamic_cast<AUserPlayerController*>(It->Get());
		if (playerController)
		{			
			auto playerState = dynamic_cast<AMainPlayerState*>(playerController->GetPlayerState());
			if (playerState)
				playerState->SetFactionCharacterCount();
		}
	}
}

TArray<int32> AMainGameState::GetFactionCharacterCount()
{	
	return factionCharacterCount;
}

int32 AMainGameState::GetCharacterCount()
{
	return characterCount;
}