// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BanditKing.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API ABanditKing : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	AnimState currentAnimState = AnimState::EQUIP;
	AnimState prevAnimState = AnimState::EQUIP;

public:
	// Sets default values for this character's properties
	ABanditKing();


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

	UPROPERTY(VisibleAnywhere, Category = WEAPON)
		class AWeapon* weapon;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* hpBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* lockOnWidget;

	UPROPERTY()
		class UBanditKingAnimInstance* banditKingAnim;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> bloodArr;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> explosionEffectArr;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* attackSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* attackSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* explosionSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* explosionSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* footWalkSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* footWalkSoundComponent;

	UPROPERTY(EditAnywhere, Category = Stat)
		bool bWrite = true;


	FTimerHandle parryTimerHandle = {};
	FTimerHandle hpRegenerationTimerHandle = {};
	FTimerHandle staminaRegenerationTimerHandle = {};
	FTimerHandle deadTimerHandle = {};
private:
	//Input Function

	void LookUp(float axisValue);
	void Turn(float axisValue);
	//void Run(float axisValue);
	//void Equip();
	//void LockOn();
	//void SetSkillMode();

	void ParryToGuard();

	//void OnAssetLoadCompleted();
	//void OnHeadAssetLoadCompleted();
	void Dead();
	void SetDamage(float damage);
	void SetStaminaDecrease(float amount);
	void BloodEffect(FVector);

	//void OnHpRegen();
	void OnStaminaRegen();

	void LockOnTick();

	//void DecreaseMainGameFactionCount();

public:
	void MoveForward(float axisValue);
	void MoveRight(float axisValue);
	void TakeStun();
	void Guard(float axisValue);
	void Run(float axisValue);

	void Attack();
	void SkillA();
	void SkillB();

	void SetLockOnTarget(ABaseCharacter*);
	void ExplosionAttack();

	//void InitCharacter(FName characterName, float hp);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);


	const struct FCharacterDataTable& GetCharacterStatData();
	AnimState GetAnimState();
	ABaseCharacter* GetLockOnTarget();
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bInputCheck;

	bool bSkillMode = false;
	int32 stunStack = 0;
	ABaseCharacter* lockOnTarget = nullptr;

	FSoftObjectPath characterAssetToLoad = FSoftObjectPath(nullptr);
	FSoftObjectPath characterHeadAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> assetStreamingHandle;
	int32 currentCombo;
	int32 maxCombo = 2;

	bool bHpRegen = true;
	bool bStaminaRegen = true;
};
