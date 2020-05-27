// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM()
enum AnimState
{
	GENERAL UMETA(DisplayName = "General"),
	EQUIP UMETA(DisplayName = "Equip"),
	SKILLMODE UMETA(DisplayName = "SkillMode"),
	ATTACKING UMETA(DisplayName = "Attacking"),
	RUN UMETA(DisplayName = "Run"),
	DODGE UMETA(DisplayName = "Dodge"),
	HIT UMETA(DisplayName = "Hit"),
	GUARD UMETA(DisplayName = "Guard"),
	PARRY UMETA(DisplayName = "Parry"),
	PARRYING UMETA(DisplayName = "Parrying"),
	STUN UMETA(DisplayName = "Stun"),
	DEAD UMETA(DisplayName = "Dead")
};

UCLASS(abstract)
class PROJECT1_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser) override;

	virtual void PossessedBy(AController* newController) override;

	virtual void Attack();
	virtual void SkillA();
	virtual void SkillB();

	virtual void MoveForward(float);
	virtual void MoveRight(float);
	virtual void Run(float);
	virtual void TakeStun();

	virtual void LockOn();
	virtual void SetCanMove(bool flag);
	virtual void SetLockOnTarget(ABaseCharacter* otherCharacter);
	virtual ABaseCharacter* GetLockOnTarget();

	virtual AnimState GetAnimState();

	virtual const struct FCharacterDataTable& GetCharacterStatData();

	virtual bool GetWritePermit() { return false; }

	virtual void ShakeEffect(float);
};
