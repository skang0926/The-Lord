// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Animation/AnimInstance.h"
#include "GruxAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FAttackCheckDelegate);

/**
 * 
 */
UCLASS()
class PROJECT1_API UGruxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UGruxAnimInstance();
		
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlayRaseAttackMontage();

	FAttackCheckDelegate OnAttackCheck;
	
	void SetDead();

private:
	UFUNCTION()
	void AnimNotify_AttackCheck();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float CharacterSpeed;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GruxAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GruxRaseAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GruxDeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool Dead = false;
};
