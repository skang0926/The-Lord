// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "HumanCharacter.h"
#include "Animation/AnimInstance.h"
#include "HumanAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FSoundPlayDelegate);
DECLARE_MULTICAST_DELEGATE(FFootWalkSoundPlayDelegate);
DECLARE_MULTICAST_DELEGATE(FCountAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextSkillAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNewAttack);
DECLARE_MULTICAST_DELEGATE(FAttackCheck);

/**
 * 
 */
UCLASS()
class PROJECT1_API UHumanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHumanAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void PlayInPlaceAttackMontage();
	void PlaySkillAttackMontage();
	void PlayRunAttackMontage();

	void PlayFwdHitMontage();
	void PlayBwdHitMontage();
	void PlayRightHitMontage();
	void PlayLeftHitMontage();

	void PlayGuardMontage();

	void PlayParryMontage();
	void PlayStunMontage();

	void PlaySkillAMontage();
	void PlaySkillARunMontage();
	void PlaySkillBMontage();

	void JumpToAttackMontageSection(int32 newSection);
	void JumpToCountAttackMontageSection();
	void JumpToSkillAttackMontageSection(int32 newSection);
	bool PlayDodgeMontage();

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FSoundPlayDelegate OnSoundPlay;
	FFootWalkSoundPlayDelegate OnFootWalkSound;
	FCountAttackDelegate OnCountAttack;
	FOnNextSkillAttackDelegate OnNextSkillAttack;
	FOnNewAttack OnNewAttack;
	FAttackCheck OnAttackCheck;

private:
	UFUNCTION()
	void AnimNotify_InputCheck();

	UFUNCTION()
	void AnimNotify_SoundPlay();

	UFUNCTION()
	void AnimNotify_FootWalkSound();

	UFUNCTION()
	void AnimNotify_CountAttack();

	UFUNCTION()
	void AnimNotify_OnNextSkillAttack();

	UFUNCTION()
	void AnimNotify_NewAttack();

	UFUNCTION()
	void AnimNotify_AttackCheck();

	FName GetAttackMontageSectionName(int32 section);
	void SetDirValue();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float CharacterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float ForwardValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float RightValue;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<AnimState> State = AnimState::GENERAL;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool isFalling;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandInPlaceAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandPierceAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandForwardAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RunAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandFwdDodgeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandBwdDodgeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandRightDodgeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandLeftDodgeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandFwdHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandBwdHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandRightHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandLeftHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandGuard;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandNextGuard;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandParry;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwoHandStun;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillA;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillARun;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SkillB;
};
