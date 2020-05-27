// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "BanditKing.h"
#include "Animation/AnimInstance.h"
#include "BanditKingAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FSoundPlayDelegate);
DECLARE_MULTICAST_DELEGATE(FFootWalkSoundPlayDelegate);
DECLARE_MULTICAST_DELEGATE(FCountAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNewAttack);
DECLARE_MULTICAST_DELEGATE(FOffAttack);
DECLARE_MULTICAST_DELEGATE(FAttackCheck);
DECLARE_MULTICAST_DELEGATE(FPhaseChange);

/**
 * 
 */
UCLASS()
class PROJECT1_API UBanditKingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBanditKingAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayGeneralAttackMontage();
	void PlayComboAttackMontage();
	void PlayDodgeAttackMontage();
	void PlaySkillDodgeAttackMontage();
	void PlayGuardMontage();
	void PlayStunMontage();
	void PlayPhaseChangeMontage();
public:


	FSoundPlayDelegate OnSoundPlay;
	FFootWalkSoundPlayDelegate OnFootWalkSound;
	FCountAttackDelegate OnCountAttack;
	FOnNewAttack OnNewAttack;
	FOffAttack OffAttack;
	FAttackCheck OnAttackCheck;
	FPhaseChange OnPhaseChange;


private:

	UFUNCTION()
	void AnimNotify_SoundPlay();

	UFUNCTION()
	void AnimNotify_FootWalkSound();

	UFUNCTION()
	void AnimNotify_CountAttack();

	UFUNCTION()
	void AnimNotify_NewAttack();

	UFUNCTION()
	void AnimNotify_OffAttack();

	UFUNCTION()
	void AnimNotify_AttackCheck();

	UFUNCTION()
	void AnimNotify_PhaseChange();


	void SetDirValue();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float characterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float forwardValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float rightValue;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		TEnumAsByte<AnimState> state = AnimState::GENERAL;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* generalAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* comboAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* dodgeAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* skillDodgeAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* stunMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* phaseChangeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* guardMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* nextGuardMontage;

};
