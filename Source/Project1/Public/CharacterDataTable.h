// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CharacterDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterDataTable() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 faction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float attackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float maxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int mainAssetNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int headAssetNumber;
};