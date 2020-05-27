// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "CharacterDataTable.h"
#include "CharacterGameInstance.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterStatComponent::InitData(const FName& characterName, float hp)
{
	auto gameInstance = Cast<UCharacterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		
	characterStatData = gameInstance->GetCharacterData(characterName);

	currentHp = hp;
	currentStamina = characterStatData->stamina;
}

void UCharacterStatComponent::SetCurrentHp(float newHp)
{
	currentHp = newHp;
	OnHpChange.Broadcast();
}

float UCharacterStatComponent::GetCurrentHp()
{
	return currentHp;
}

float UCharacterStatComponent::GetHpRatio()
{
	return currentHp / (*characterStatData).hp;
}

void UCharacterStatComponent::SetCurrentStamina(float newStamina)
{
	currentStamina = newStamina;
	OnStaminaChanged.Broadcast();
}

float UCharacterStatComponent::GetCurrentStamina()
{
	return currentStamina;
}

float UCharacterStatComponent::GetStaminaRatio()
{
	return currentStamina / (*characterStatData).stamina;
}

const FCharacterDataTable& UCharacterStatComponent::GetCharacterStatData()
{
	return *characterStatData;
}