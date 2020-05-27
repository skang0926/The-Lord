// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BaseCharacter.h"
#include "TutorialCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API ATutorialCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	AnimState currentAnimState = AnimState::GENERAL;
	AnimState prevAnimState = AnimState::GENERAL;

public:
	// Sets default values for this character's properties
	ATutorialCharacter();


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

	virtual void PossessedBy(AController* newController) override;

	virtual void Attack();
public:
	// Component
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UCharacterStatComponent* characterStat;

	UPROPERTY(EditAnywhere, Category = Stat)
		FName characterName;

	UPROPERTY(EditAnywhere, Category = Stat)
		float hp = 150.f;

	UPROPERTY(VisibleAnywhere, Category = HEAD)
		USkeletalMeshComponent* head;

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
		class AWeapon* weapon;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* hpBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* lockOnWidget;

	UPROPERTY()
		class UHumanAnimInstance* humanAnim;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> bloodArr;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> electronicEffectArr;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* attackSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* attackSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* electronicSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* electronicSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* footWalkSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* footWalkSoundComponent;

	

	FTimerHandle parryTimerHandle = {};
	FTimerHandle hpRegenerationTimerHandle = {};
	FTimerHandle staminaRegenerationTimerHandle = {};
	FTimerHandle tutorialTimerHandle = {};
	FTimerHandle hpBarTimerHandle = {};

	int32 moveForwardValue;
	int32 moveRightValue;

private:
	//Input Function

	void LookUp(float axisValue);
	void Turn(float axisValue);
	void Run(float axisValue);
	void Equip();
	void LockOn();
	void SetSkillMode();
	void ParryToGuard();

	void SkillA();
	void SkillB();

	void SetDamage(float damage);
	void SetStaminaDecrease(float amount);
	void BloodEffect(FVector);

	void OnHpRegen();
	void OnStaminaRegen();

	void LockOnTick();

	void TutorialUpdate();

	void OffHpBarWidget();
public:
	void MoveForward(float axisValue);
	void MoveRight(float axisValue);	
	void Guard(float axisValue);

	void TakeStun();
	void ShakeEffect(float);

	void ElectronicAttack();

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);

	void SetLockOnTarget(ABaseCharacter* otherCharacter);

	UFUNCTION(BlueprintCallable, Category = State)
		void SetCanMove(bool flag);


	const struct FCharacterDataTable& GetCharacterStatData();
	AnimState GetAnimState();
	ABaseCharacter* GetLockOnTarget();

	class ATutorialPlayerController* playerController;
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bInputCheck;

	ABaseCharacter* lockOnTarget;


	int32 currentCombo;
		

	bool bHpRegen = true;
	bool bStaminaRegen = true;

	bool canInput = true;
};
