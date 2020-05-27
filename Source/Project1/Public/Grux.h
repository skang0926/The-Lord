// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "Grux.generated.h"

UCLASS()
class PROJECT1_API AGrux : public ABaseCharacter
{
	GENERATED_BODY()

private:
	AnimState currentAnimState = AnimState::GENERAL;

public:
	// Sets default values for this character's properties
	
	AGrux();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents();
	virtual float TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser);


	void MoveForward(float axisValue);
	void MoveRight(float axisValue);
	
	void LookUp(float axisValue);
	void Turn(float axisValue);
	
	void Attack();
	void RaseAttack();	

	AnimState GetAnimState();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);

	void AttackCheck();

	void BloodEffect(FVector);	
	   
	void PlayRaseSound();

	void SetDamage(float damage);

	const struct FCharacterDataTable& GetCharacterStatData();
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;	

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCharacterStatComponent* characterStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		FName characterName;


	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* hpBarWidget;

	UPROPERTY()
		class UGruxAnimInstance* gruxAnim;

	UPROPERTY(VisibleAnywhere)
	TArray<UParticleSystemComponent*> bloodArr;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* hitSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* hitSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* guardSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* guardSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* parrySoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* parrySoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* attackSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* attackSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* raseSound;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> effectComponentArr;

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float maxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float currentHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 faction = 0;
};