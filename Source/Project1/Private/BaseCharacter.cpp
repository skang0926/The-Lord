// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "CharacterGameInstance.h"
#include "CharacterDataTable.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

float ABaseCharacter::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);
	return 0;
}

void ABaseCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

void ABaseCharacter::Attack()
{

}

void ABaseCharacter::SkillA()
{

}

void ABaseCharacter::SkillB()
{

}

void ABaseCharacter::MoveForward(float axisValue)
{

}

void ABaseCharacter::MoveRight(float axisValue)
{

}

void ABaseCharacter::Run(float axisValue)
{

}


void ABaseCharacter::TakeStun()
{

}

void ABaseCharacter::LockOn()
{

}

void ABaseCharacter::ShakeEffect(float scale)
{

}

ABaseCharacter* ABaseCharacter::GetLockOnTarget()
{
	return nullptr;
}

void ABaseCharacter::SetLockOnTarget(ABaseCharacter* otherCharacter)
{
	
}

void ABaseCharacter::SetCanMove(bool flag)
{

}

AnimState ABaseCharacter::GetAnimState()
{
	return AnimState::GENERAL;
}

const FCharacterDataTable& ABaseCharacter::GetCharacterStatData()
{
	auto gameInstance = Cast<UCharacterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto defaultData = gameInstance->GetCharacterData(TEXT("Player"));
	return *defaultData;
}