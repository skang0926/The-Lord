// Fill out your copyright notice in the Description page of Project Settings.


#include "BanditKingAnimInstance.h"

UBanditKingAnimInstance::UBanditKingAnimInstance()
{
	characterSpeed = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> GENERAL_ATTACK_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditKingAttack_Montage.BanditKingAttack_Montage"));
	if (GENERAL_ATTACK_MONTAGE.Succeeded())
	{
		generalAttackMontage = GENERAL_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> COMBO_ATTACK_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditComboAttack.BanditComboAttack"));
	if (COMBO_ATTACK_MONTAGE.Succeeded())
	{
		comboAttackMontage = COMBO_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_ATTACK_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditKingDodgeAttack_Montage.BanditKingDodgeAttack_Montage"));
	if (DODGE_ATTACK_MONTAGE.Succeeded())
	{
		dodgeAttackMontage = DODGE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_DODGE_ATTACK_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditKing2PhaseSkillA.BanditKing2PhaseSkillA"));
	if (SKILL_DODGE_ATTACK_MONTAGE.Succeeded())
	{
		skillDodgeAttackMontage = SKILL_DODGE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> STUN_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditKingStun_Montage.BanditKingStun_Montage"));
	if (STUN_MONTAGE.Succeeded())
	{
		stunMontage = STUN_MONTAGE.Object;
	}	

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PHASECHANGE_MONTAGE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Animation/Montage/BanditKingPhaseChange_Montage.BanditKingPhaseChange_Montage"));
	if (PHASECHANGE_MONTAGE.Succeeded())
	{
		phaseChangeMontage = PHASECHANGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GUARD_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandedGuard.TwoHandedGuard"));
	if (GUARD_MONTAGE.Succeeded())
	{
		guardMontage = GUARD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NEXT_GUARD_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandedNextGuard.TwoHandedNextGuard"));
	if (NEXT_GUARD_MONTAGE.Succeeded())
	{
		nextGuardMontage = NEXT_GUARD_MONTAGE.Object;
	}

}

void UBanditKingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = dynamic_cast<ABanditKing*>(TryGetPawnOwner());

	if (!::IsValid(pawn)) return;

	SetDirValue();

	state = pawn->GetAnimState();
}

void UBanditKingAnimInstance::PlayGeneralAttackMontage()
{
	Montage_Play(generalAttackMontage, 1.0f);
}

void UBanditKingAnimInstance::PlayComboAttackMontage()
{
	Montage_Play(comboAttackMontage, 1.0f);
}

void UBanditKingAnimInstance::PlayDodgeAttackMontage()
{
	Montage_Play(dodgeAttackMontage, 1.0f);
}

void UBanditKingAnimInstance::PlaySkillDodgeAttackMontage()
{
	Montage_Play(skillDodgeAttackMontage, 1.0f);
}

void UBanditKingAnimInstance::PlayStunMontage()
{
	Montage_Play(stunMontage, 1.0f);
}

void UBanditKingAnimInstance::PlayPhaseChangeMontage()
{
	if (Montage_IsPlaying(phaseChangeMontage)) return;

	Montage_Play(phaseChangeMontage, 1.0f);
}

void UBanditKingAnimInstance::PlayGuardMontage()
{
	if (Montage_IsPlaying(guardMontage))
	{
		Montage_Play(nextGuardMontage, 1.0f);
		return;
	}
	Montage_Play(guardMontage, 1.0f);
}




void UBanditKingAnimInstance::AnimNotify_SoundPlay()
{
	OnSoundPlay.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_FootWalkSound()
{
	OnFootWalkSound.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_CountAttack()
{	
	OnCountAttack.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_NewAttack()
{
	OnNewAttack.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_OffAttack()
{
	OffAttack.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheck.Broadcast();
}

void UBanditKingAnimInstance::AnimNotify_PhaseChange()
{
	OnPhaseChange.Broadcast();
}

void UBanditKingAnimInstance::SetDirValue()
{
	auto pawn = dynamic_cast<ABaseCharacter*>(TryGetPawnOwner());

	if (!::IsValid(pawn)) return;

	characterSpeed = pawn->GetVelocity().Size();

	FVector vDir = pawn->GetVelocity();
	vDir = vDir.GetSafeNormal();


	FVector forwardVec = pawn->GetActorForwardVector();
	float radianForwardVec = FVector::DotProduct(forwardVec, vDir);
	float degreeX = FMath::RadiansToDegrees(acos(radianForwardVec));


	FVector rightVec = pawn->GetActorRightVector();
	float radianRightVec = FVector::DotProduct(rightVec, vDir);
	float degreeY = FMath::RadiansToDegrees(acos(radianRightVec));


	if (degreeY > 90.f)
	{
		degreeX *= -1.f;
	}

	if (characterSpeed == 0.f)
	{
		forwardValue = 0.f;
		rightValue = 0.f;
	}
	else
	{
		if (abs(degreeX) <= 85.f)
		{
			forwardValue = 1.f;
		}
		else if (abs(degreeX) >= 95.f)
		{
			forwardValue = -1.f;
		}
		else
		{
			forwardValue = 0.f;
		}

		if (degreeX > 5.f && degreeX < 175.f)
		{
			rightValue = 1.f;
		}
		else if (degreeX > -175.f && degreeX < -5.f)
		{
			rightValue = -1.f;
		}
		else
		{
			rightValue = 0.f;
		}

	}
}