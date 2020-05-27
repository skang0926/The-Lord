// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxAnimInstance.h"
#include "Grux.h"
UGruxAnimInstance::UGruxAnimInstance()
{
	CharacterSpeed = 0.f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> GRUX_ATTACK_MONTAGE(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/GruxAttackMontage.GruxAttackMontage"));
	if (GRUX_ATTACK_MONTAGE.Succeeded())
	{
		GruxAttackMontage = GRUX_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GRUX_RASE_ATTACK_MONTAGE(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/GruxRaseAttackMontage.GruxRaseAttackMontage"));
	if (GRUX_RASE_ATTACK_MONTAGE.Succeeded())
	{
		GruxRaseAttackMontage = GRUX_RASE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GRUX_DEAD_MONTAGE(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/GruxDeadMontage.GruxDeadMontage"));
	if (GRUX_DEAD_MONTAGE.Succeeded())
	{
		GruxDeadMontage = GRUX_DEAD_MONTAGE.Object;
	}
}


void UGruxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = dynamic_cast<AGrux*>(TryGetPawnOwner());

	if (!::IsValid(Pawn)) return;

	CharacterSpeed = Pawn->GetVelocity().Size();
}

void UGruxAnimInstance::AnimNotify_AttackCheck()
{
	UE_LOG(LogClass, Warning, TEXT("Notify Attack Check"));
	OnAttackCheck.Broadcast();
}

void UGruxAnimInstance::PlayAttackMontage()
{
	if (!IsAnyMontagePlaying())	
	{
		Montage_Play(GruxAttackMontage, 1.0f);
	}
}

void UGruxAnimInstance::PlayRaseAttackMontage()
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(GruxRaseAttackMontage, 1.0f);
	}
}

void UGruxAnimInstance::SetDead()
{
	Dead = true;	
	Montage_Play(GruxDeadMontage, 1.0f);
}
