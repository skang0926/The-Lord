// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROJECT1_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetCollision(bool);
	virtual void OnEffect(FVector);
	virtual void PlayGuardSound();
	virtual void PlayParrySound();
	virtual void SetWeaponParticle(bool);
};

