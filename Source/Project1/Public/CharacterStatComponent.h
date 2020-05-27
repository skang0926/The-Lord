// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate)
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate)
DECLARE_MULTICAST_DELEGATE(FOnStaminaChangedDelegate)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT1_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void InitData(const FName& characterName, float  hp);
	void SetCurrentHp(float newHp);
	float GetCurrentHp();
	float GetHpRatio();

	void SetCurrentStamina(float newStamina);
	float GetCurrentStamina();
	float GetStaminaRatio();

	const struct FCharacterDataTable& GetCharacterStatData();

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;
	FOnStaminaChangedDelegate OnStaminaChanged;

	struct FCharacterDataTable* characterStatData = nullptr;
private:
//	struct FCharacterDataTable* characterStatData = nullptr;
	   
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = ture))
		float currentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = ture))
		float currentStamina;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FOnHPIsZeroDelegate OnHpChange;
};
