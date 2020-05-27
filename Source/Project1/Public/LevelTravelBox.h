// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Engine/TriggerBox.h"
#include "LevelTravelBox.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API ALevelTravelBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ALevelTravelBox();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditAnywhere, Category = Level)
	FName nextLevelName;

	UPROPERTY(EditAnywhere, Category = Level)
	FVector nextLevelLocation;

	UPROPERTY(EditAnywhere, Category = Level)
	FRotator nextLevelRotator;

private:
	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* overlapComp, AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void WritePlayerDataFile(AActor*);
	void WriteCharacterDataFile();

};
