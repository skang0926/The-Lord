// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"


UENUM()
enum FileReadWriteSetting
{
	READONLY UMETA(DisplayName = "ReadOnly"),
	WRITEONLY UMETA(DisplayName = "WriteOnly"),
	READANDWRITE UMETA(DisplayName = "ReadAndWrite")
};

UCLASS()
class PROJECT1_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACharacterSpawner();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))	
	TEnumAsByte<FileReadWriteSetting> readWriteSetting = FileReadWriteSetting::READONLY;

	UPROPERTY(VisibleAnywhere)
		class AHumanCharacter* playerCharacter = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReadCharacterDataFile();	
	void WriteCharacterDataFile();

	void ReadPlayerDataFile();
	void WritePlayerDataFile(AActor*);

	void SaveGame();
};
