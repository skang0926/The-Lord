// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAnimInstance.h"


UHumanAnimInstance::UHumanAnimInstance()
{
	CharacterSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_INPLACE_ATTACK_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandInPlaceAttackMontage.TwoHandInPlaceAttackMontage"));
	if (TWO_HAND_INPLACE_ATTACK_MONTAGE.Succeeded())
	{
		TwoHandInPlaceAttackMontage = TWO_HAND_INPLACE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_PIERCE_ATTACK_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandPierce.TwoHandPierce"));
	if (TWO_HAND_PIERCE_ATTACK_MONTAGE.Succeeded())
	{
		TwoHandPierceAttackMontage = TWO_HAND_PIERCE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_FORWARD_ATTACK_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandForwardAttackMontage.TwoHandForwardAttackMontage"));
	if (TWO_HAND_FORWARD_ATTACK_MONTAGE.Succeeded())
	{
		TwoHandForwardAttackMontage = TWO_HAND_FORWARD_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_ATTACK_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/WARRIOR_SWORD_ANIMSET/Sword_Warrior/Animation_RM/Montage/SkillModeAttackMontage.SkillModeAttackMontage"));
	if (SKILL_ATTACK_MONTAGE.Succeeded())
	{
		SkillAttackMontage = SKILL_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RUN_ATTACK_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/WARRIOR_SWORD_ANIMSET/Sword_Warrior/Animation_RM/Montage/RunAttack.RunAttack"));
	if (RUN_ATTACK_MONTAGE.Succeeded())
	{
		RunAttackMontage = RUN_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_FWD_DODGE_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_FwdDodge.TwoHanded_FwdDodge"));
	if (TWO_HAND_FWD_DODGE_MONTAGE.Succeeded())
	{
		TwoHandFwdDodgeMontage = TWO_HAND_FWD_DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_BWD_DODGE_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_BwdDodge.TwoHanded_BwdDodge"));
	if (TWO_HAND_BWD_DODGE_MONTAGE.Succeeded())
	{
		TwoHandBwdDodgeMontage = TWO_HAND_BWD_DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_RIGHT_DODGE_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_RightDodge.TwoHanded_RightDodge"));
	if (TWO_HAND_RIGHT_DODGE_MONTAGE.Succeeded())
	{
		TwoHandRightDodgeMontage = TWO_HAND_RIGHT_DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_LEFT_DODGE_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_LeftDodge.TwoHanded_LeftDodge"));
	if (TWO_HAND_LEFT_DODGE_MONTAGE.Succeeded())
	{
		TwoHandLeftDodgeMontage = TWO_HAND_LEFT_DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_GUARD_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandedGuard.TwoHandedGuard"));
	if (TWO_HAND_GUARD_MONTAGE.Succeeded())
	{
		TwoHandGuard = TWO_HAND_GUARD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_NEXT_GUARD_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHandedNextGuard.TwoHandedNextGuard"));
	if (TWO_HAND_GUARD_MONTAGE.Succeeded())
	{
		TwoHandNextGuard = TWO_HAND_NEXT_GUARD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_PARRY_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_ParrySuccess.TwoHanded_ParrySuccess"));
	if (TWO_HAND_PARRY_MONTAGE.Succeeded())
	{
		TwoHandParry = TWO_HAND_PARRY_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_STUN_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/TwoHanded_Stun.TwoHanded_Stun"));
	if (TWO_HAND_STUN_MONTAGE.Succeeded())
	{
		TwoHandStun = TWO_HAND_STUN_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_FORWARD_HIT_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/FwdHitReaction.FwdHitReaction"));
	if (TWO_HAND_FORWARD_HIT_MONTAGE.Succeeded())
	{
		TwoHandFwdHitMontage = TWO_HAND_FORWARD_HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_BACKWARD_HIT_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/BwdHitReaction.BwdHitReaction"));
	if (TWO_HAND_BACKWARD_HIT_MONTAGE.Succeeded())
	{
		TwoHandBwdHitMontage = TWO_HAND_BACKWARD_HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_RIGHT_HIT_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/RightHitReaction.RightHitReaction"));
	if (TWO_HAND_RIGHT_HIT_MONTAGE.Succeeded())
	{
		TwoHandRightHitMontage = TWO_HAND_RIGHT_HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TWO_HAND_LEFT_HIT_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/LeftHitReaction.LeftHitReaction"));
	if (TWO_HAND_LEFT_HIT_MONTAGE.Succeeded())
	{
		TwoHandLeftHitMontage = TWO_HAND_LEFT_HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILLA_MONTAGE(
		TEXT("AnimMontage'/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/SkillA_Montage.SkillA_Montage'"));
	if (SKILLA_MONTAGE.Succeeded())
	{
		SkillA = SKILLA_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILLARUN_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/SkillARun_Montage.SkillARun_Montage"));
	if (SKILLARUN_MONTAGE.Succeeded())
	{
		SkillARun = SKILLARUN_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILLB_MONTAGE(
		TEXT("/Game/Characters/GhostLady_S2/Animations/TwoHandedAnim/Montage/SkillB_Montage.SkillB_Montage"));
	if (SKILLB_MONTAGE.Succeeded())
	{
		SkillB = SKILLB_MONTAGE.Object;
	}
}

void UHumanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = dynamic_cast<ABaseCharacter*>(TryGetPawnOwner());
	
	if (!::IsValid(Pawn)) return;

	

	SetDirValue();	
	
	State = Pawn->GetAnimState();

	if (Pawn->GetMovementComponent()->IsFalling())
		isFalling = true;
	else
		isFalling = false;
}

void UHumanAnimInstance::PlayInPlaceAttackMontage()
{
	if (IsAnyMontagePlaying()) return;

	if (ForwardValue < 0 && (RightValue == 0))
	{
		Montage_Play(TwoHandPierceAttackMontage, 1.0f);		
	}
	else if (ForwardValue > 0 && (RightValue == 0))
	{	
		Montage_Play(TwoHandForwardAttackMontage, 1.0f);
	}
	else
	{
		Montage_Play(TwoHandInPlaceAttackMontage, 1.0f);
	}
}

void UHumanAnimInstance::PlaySkillAttackMontage()
{
	if (IsAnyMontagePlaying()) return;
	
	Montage_Play(SkillAttackMontage, 1.0f);
}

void UHumanAnimInstance::PlayRunAttackMontage()
{
	if (IsAnyMontagePlaying()) return;

	Montage_Play(RunAttackMontage, 1.0f);
}

void UHumanAnimInstance::PlaySkillAMontage()
{
	Montage_Play(SkillA, 1.0f);
}

void UHumanAnimInstance::PlaySkillARunMontage()
{
	Montage_Play(SkillARun, 1.0f);
}

void UHumanAnimInstance::PlaySkillBMontage()
{
	Montage_Play(SkillB, 1.0f);
}

void UHumanAnimInstance::JumpToAttackMontageSection(int32 newSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(newSection), TwoHandInPlaceAttackMontage);
}

void UHumanAnimInstance::JumpToSkillAttackMontageSection(int32 newSection)
{	
	Montage_JumpToSection(GetAttackMontageSectionName(newSection), SkillAttackMontage);
}


void UHumanAnimInstance::JumpToCountAttackMontageSection()
{		
	Montage_JumpToSection(GetAttackMontageSectionName(2), TwoHandParry);
}

void UHumanAnimInstance::AnimNotify_InputCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UHumanAnimInstance::AnimNotify_OnNextSkillAttack()
{
	UE_LOG(LogClass, Warning, TEXT("notify?"));
	OnNextSkillAttack.Broadcast();
}

void UHumanAnimInstance::AnimNotify_SoundPlay()
{
	OnSoundPlay.Broadcast();
}

void UHumanAnimInstance::AnimNotify_FootWalkSound()
{
	OnFootWalkSound.Broadcast();
}

void UHumanAnimInstance::AnimNotify_CountAttack()
{	
	UE_LOG(LogClass, Warning, TEXT("Count Notify"));
	OnCountAttack.Broadcast();
}

void UHumanAnimInstance::AnimNotify_NewAttack()
{
	OnNewAttack.Broadcast();
}

void UHumanAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheck.Broadcast();
}

FName UHumanAnimInstance::GetAttackMontageSectionName(int32 section)
{	
	return FName(*FString::Printf(TEXT("Attack%d"), section));
}


bool UHumanAnimInstance::PlayDodgeMontage()
{
	if (ForwardValue > 0 && (RightValue == 0))
	{
		if (!Montage_IsPlaying(TwoHandFwdDodgeMontage))		
		{
			Montage_Play(TwoHandFwdDodgeMontage, 1.0f);
		}
	}
	else if (ForwardValue < 0 && (RightValue == 0))
	{
		if (!Montage_IsPlaying(TwoHandBwdDodgeMontage))
		{
			Montage_Play(TwoHandBwdDodgeMontage, 1.0f);
		}
	}
	else if (RightValue > 0 && (ForwardValue == 0))
	{
		if (!Montage_IsPlaying(TwoHandRightDodgeMontage))
		{
			Montage_Play(TwoHandRightDodgeMontage, 1.0f);
		}
	}
	else if (RightValue < 0 && (ForwardValue == 0))
	{
		if (!Montage_IsPlaying(TwoHandLeftDodgeMontage))
		{
			Montage_Play(TwoHandLeftDodgeMontage, 1.0f);
		}
	}
	else
	{
		return false;
	}

	return true;
}

void UHumanAnimInstance::PlayFwdHitMontage()
{
	Montage_Play(TwoHandFwdHitMontage, 1.0f);
}
void UHumanAnimInstance::PlayBwdHitMontage()
{
	Montage_Play(TwoHandBwdHitMontage, 1.0f);
}
void UHumanAnimInstance::PlayRightHitMontage()
{
	Montage_Play(TwoHandRightHitMontage, 1.0f);
}
void UHumanAnimInstance::PlayLeftHitMontage()
{
	Montage_Play(TwoHandLeftHitMontage, 1.0f);
}

void UHumanAnimInstance::PlayGuardMontage()
{
	if (Montage_IsPlaying(TwoHandGuard))
	{
		Montage_Play(TwoHandNextGuard, 1.0f);
		return;
	}	
	Montage_Play(TwoHandGuard, 1.0f);
}

void UHumanAnimInstance::PlayParryMontage()
{
	Montage_Play(TwoHandParry, 1.0f);
}

void UHumanAnimInstance::PlayStunMontage()
{
	Montage_Play(TwoHandStun, 1.0f);
}

void UHumanAnimInstance::SetDirValue()
{
	auto Pawn = dynamic_cast<ABaseCharacter*>(TryGetPawnOwner());

	if (!::IsValid(Pawn)) return;

	CharacterSpeed = Pawn->GetVelocity().Size();

	FVector vDir = Pawn->GetVelocity();
	vDir = vDir.GetSafeNormal();


	FVector forwardVec = Pawn->GetActorForwardVector();
	float radianForwardVec = FVector::DotProduct(forwardVec, vDir);
	float degreeX = FMath::RadiansToDegrees(acos(radianForwardVec));


	FVector rightVec = Pawn->GetActorRightVector();
	float radianRightVec = FVector::DotProduct(rightVec, vDir);
	float degreeY = FMath::RadiansToDegrees(acos(radianRightVec));


	if (degreeY > 90.f)
	{
		degreeX *= -1.f;
	}

	if (CharacterSpeed == 0.f)
	{
		ForwardValue = 0.f;
		RightValue = 0.f;
	}
	else
	{
		if (abs(degreeX) <= 85.f)
		{
			ForwardValue = 1.f;			
		}
		else if (abs(degreeX) >= 95.f)
		{
			ForwardValue = -1.f;
		}
		else
		{
			ForwardValue = 0.f;
		}

		if (degreeX > 5.f && degreeX < 175.f)
		{
			RightValue = 1.f;
		}
		else if (degreeX > -175.f && degreeX < -5.f)
		{
			RightValue = -1.f;
		}
		else
		{
			RightValue = 0.f;
		}

	}
}