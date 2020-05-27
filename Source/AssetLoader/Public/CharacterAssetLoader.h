// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterAssetLoader.generated.h"

/**
 * 
 */
UCLASS(config = CharacterAssets)
class ASSETLOADER_API UCharacterAssetLoader : public UObject
{
	GENERATED_BODY()
public:
	UCharacterAssetLoader();

	UPROPERTY(config)
		TArray<FSoftObjectPath>characterAssets;
};
