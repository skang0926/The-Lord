// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/Actor.h"
#include "NewGameSetting.generated.h"

UCLASS()
class PROJECT1_API ANewGameSetting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewGameSetting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DefaultWriteCharacterDataFile(FString);
	void ReadCharacterDataFile(FString);

};
