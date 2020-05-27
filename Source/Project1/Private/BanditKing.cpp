// Fill out your copyright notice in the Description page of Project Settings.


#include "BanditKing.h"
#include "BanditKingAnimInstance.h"
#include "OneHandSword.h"
#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "CharacterStatComponent.h"
#include "CharacterGameInstance.h"
#include "CharacterAssetLoader.h"
#include "HumanCharacterWidget.h"
#include "UserPlayerController.h"
#include "UserHUDWidget.h"
#include "MainGameState.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "CameraShakeEffect.h"
#include "BanditKingAIController.h"

ABanditKing::ABanditKing()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	characterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("STAT"));
	
	attackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ATTACKSOUND"));
	explosionSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ELECTRONICSOUND"));
	footWalkSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FOOTWALKSOUND"));	

	characterName = TEXT("BanditKing");

	//Set Attachment
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(
		TEXT("/Game/Characters/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Tusk.SK_CharM_Tusk"));
	if (SK_BASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BASE.Object);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	springArm->TargetArmLength = 400.f;
	springArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	springArm->ProbeChannel = ECollisionChannel::ECC_Pawn;
	springArm->ProbeSize = 1.0f;
	springArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	   	 

	static ConstructorHelpers::FClassFinder<UAnimInstance> ONEHANDSWORD_ANIM(
		TEXT("/Game/Characters/InfinityBladeWarriors/BanditKingAnimBP.BanditKingAnimBP_C"));
	if (ONEHANDSWORD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ONEHANDSWORD_ANIM.Class);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("not found AnimBP"));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_BLOOD(
		TEXT("/Game/Effect/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact"));
	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("BLOOD %d"), i);
		UParticleSystemComponent* blood = CreateDefaultSubobject<UParticleSystemComponent>(name);
		blood->SetupAttachment(GetCapsuleComponent());
		blood->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		blood->bAutoActivate = false;
		blood->SetTemplate(P_BLOOD.Object);
		blood->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
		bloodArr.Add(blood);
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_EXPLORSIONPARTICLE(TEXT("/Game/M5VFXVOL2/Particles/Explosion/Fire_Exp_00.Fire_Exp_00"));
	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("EXPLOSION %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetupAttachment(GetCapsuleComponent());
		effect->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_EXPLORSIONPARTICLE.Object);
		effect->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
		explosionEffectArr.Add(effect);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ATTACKSOUNDCUE(TEXT("/Game/Sound/A_RandSwordAttack.A_RandSwordAttack"));
	if (ATTACKSOUNDCUE.Succeeded())
	{
		attackSoundCue = ATTACKSOUNDCUE.Object;
	}

	attackSoundComponent->bAutoActivate = false;
	attackSoundComponent->SetupAttachment(GetCapsuleComponent());
	attackSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundCue> EXPLOSIONSOUNDCUE(TEXT("/Game/DarkMagic/DarkMagicCue/ExplosionSoundCue.ExplosionSoundCue"));
	if (EXPLOSIONSOUNDCUE.Succeeded())
	{
		explosionSoundCue = EXPLOSIONSOUNDCUE.Object;
	}

	explosionSoundComponent->bAutoActivate = false;
	explosionSoundComponent->SetupAttachment(GetCapsuleComponent());
	explosionSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> FOOTSOUNDCUE(TEXT("/Game/Maps/SoulCave/Sound/Cue/Footstep_Dirt_Cue.Footstep_Dirt_Cue"));
	if (FOOTSOUNDCUE.Succeeded())
	{
		footWalkSoundCue = FOOTSOUNDCUE.Object;
	}

	footWalkSoundComponent->bAutoActivate = false;
	footWalkSoundComponent->SetupAttachment(GetCapsuleComponent());
	footWalkSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));



	hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	hpBarWidget->SetupAttachment(GetMesh());

	hpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		hpBarWidget->SetWidgetClass(UI_HUD.Class);
		hpBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
		hpBarWidget->bVisible = false;
	}

	AIControllerClass = ABanditKingAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABanditKing::BeginPlay()
{
	Super::BeginPlay();

	characterStat->InitData(characterName, 1000.f);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;

	auto characterWidget = Cast<UHumanCharacterWidget>(hpBarWidget->GetUserWidgetObject());

	if (characterWidget != nullptr)
	{
		characterWidget->BindCharacterWidget(characterStat);
	}

	FName hand_rOneHandSocket(TEXT("hand_rOneHand"));
	if (GetMesh()->DoesSocketExist(hand_rOneHandSocket))
	{
		weapon = GetWorld()->SpawnActor<AOneHandSword>(AOneHandSword::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, hand_rOneHandSocket);
		weapon->SetActorScale3D(FVector(2.f, 2.f, 2.f));
		weapon->SetOwner(this);		
	}
}

// Called every frame
void ABanditKing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (lockOnTarget)
		LockOnTick();

	if (bStaminaRegen && characterStat->GetCurrentStamina() + 1.f < GetCharacterStatData().stamina && IsPlayerControlled())
		characterStat->SetCurrentStamina(characterStat->GetCurrentStamina() + 0.1f);
}

// Called to bind functionality to input
void ABanditKing::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABanditKing::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABanditKing::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABanditKing::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABanditKing::Turn);
	PlayerInputComponent->BindAxis(TEXT("Guard"), this, &ABanditKing::Guard);
	PlayerInputComponent->BindAxis(TEXT("Run"), this, &ABanditKing::Run);


	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ABanditKing::Attack);

}

void ABanditKing::PostInitializeComponents()
{
	Super::PostInitializeComponents();	

	banditKingAnim = Cast<UBanditKingAnimInstance>(GetMesh()->GetAnimInstance());

	banditKingAnim->OnMontageEnded.AddDynamic(this, &ABanditKing::OnMontageEnded);

	banditKingAnim->OnNewAttack.AddLambda([this]() -> void {
		weapon->SetCollision(true);
	});

	banditKingAnim->OffAttack.AddLambda([this]() -> void {
		weapon->SetCollision(false);
	});

	banditKingAnim->OnCountAttack.AddLambda([this]() -> void {

		if (bInputCheck)
		{
			bInputCheck = false;
		//	banditKingAnim->JumpToCountAttackMontageSection();
			weapon->SetCollision(true);
		}
	});

	banditKingAnim->OnAttackCheck.AddLambda([this]() -> void {
		if (bSkillMode)		
			ExplosionAttack();
	});

	banditKingAnim->OnSoundPlay.AddLambda([this]() -> void {
		attackSoundComponent->Play();
	});

	banditKingAnim->OnPhaseChange.AddLambda([this]() -> void {
		ExplosionAttack();
		currentAnimState = AnimState::SKILLMODE;
		prevAnimState = currentAnimState;
		bSkillMode = true;
		weapon->SetWeaponParticle(true);
		ExplosionAttack();
	});

	banditKingAnim->OnFootWalkSound.AddLambda([this]() -> void {
	//	footWalkSoundComponent->Play();
	});

	if (attackSoundCue->IsValidLowLevelFast())
	{
		attackSoundComponent->SetSound(attackSoundCue);
	}

	if (explosionSoundCue->IsValidLowLevelFast())
	{
		explosionSoundComponent->SetSound(explosionSoundCue);
	}

	if (footWalkSoundCue->IsValidLowLevelFast())
	{
	//	footWalkSoundComponent->SetSound(footWalkSoundCue);
	}
}

void ABanditKing::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

void ABanditKing::Run(float axisValue)
{
	if (!axisValue)
	{
		if (currentAnimState == AnimState::RUN)
		{
			currentAnimState = prevAnimState;
			switch (currentAnimState)
			{
			case AnimState::GENERAL:
				GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed;
				break;
			case AnimState::EQUIP:
			case AnimState::SKILLMODE:
				GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				break;
			default:
				break;
			}
		}

		return;
	}

	switch (currentAnimState)
	{
	case AnimState::GENERAL:
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		prevAnimState = currentAnimState;
		currentAnimState = AnimState::RUN;
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed + 100;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		break;
	default:
		break;
	}
}

void ABanditKing::Attack()
{
	switch (currentAnimState)
	{
	case AnimState::ATTACKING:
		if (!bInputCheck)
		{
			currentCombo++;
			bInputCheck = true;
		}
		break;
	case AnimState::PARRYING:
		bInputCheck = true;
		break;

	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		banditKingAnim->PlayGeneralAttackMontage();	
		break;
	case AnimState::RUN:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		banditKingAnim->PlayGeneralAttackMontage();
		break;
	default:
		break;
	}
}

void ABanditKing::SkillA()
{
	switch (currentAnimState)
	{
	case AnimState::EQUIP:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		banditKingAnim->PlayDodgeAttackMontage();
		break;
	case AnimState::SKILLMODE:
		if (banditKingAnim->IsAnyMontagePlaying()) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		banditKingAnim->PlaySkillDodgeAttackMontage();		
	case AnimState::RUN:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentAnimState = AnimState::ATTACKING;
		if (bSkillMode)
			banditKingAnim->PlaySkillDodgeAttackMontage();
		else
			banditKingAnim->PlayDodgeAttackMontage();
	default:
		break;
	}
}

void ABanditKing::SkillB()
{
	switch (currentAnimState)
	{
	case AnimState::ATTACKING:
		if (!bInputCheck)
		{
			currentCombo++;
			bInputCheck = true;
		}
		break;
	case AnimState::PARRYING:
		bInputCheck = true;
		break;

	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		banditKingAnim->PlayComboAttackMontage();
		break;
	case AnimState::RUN:
		if (banditKingAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentAnimState = AnimState::ATTACKING;
		//	banditKingAnim->PlayRunAttackMontage();
		//	ElectronicAttack();
	default:
		break;
	}
}

void ABanditKing::ExplosionAttack()
{
	FCollisionQueryParams Params(NAME_None, false, this);
	FHitResult hitResult;

	float range = 250.0;
	float radius = 150.f;	
	
	for (auto effect : explosionEffectArr)
	{
		if (!effect->IsActive())
		{
			effect->Activate();	
			break;
		}	
	}

	if (lockOnTarget)
	{
		lockOnTarget->ShakeEffect(20.f);
	}

	explosionSoundComponent->Play();

	TArray<FHitResult> hitResultArr;
	bool bhitResult = GetWorld()->SweepMultiByChannel(
		hitResultArr,
		GetActorLocation() + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * radius,
		GetActorLocation() + GetActorForwardVector() * range,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(radius),
		Params);


#if DRAW_DEBUG
	///////////////////debug drawing///////////////////////
	FVector traceVec = GetActorForwardVector() * range;
	FVector center = GetActorLocation() + traceVec * 0.5f;
	float halfHeight = range * 0.5f + radius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bhitResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 10.0f;


	DrawDebugCapsule(GetWorld(),
		center,
		halfHeight,
		radius,
		capsuleRot,
		drawColor,
		false,
		debugLifeTime);
	///////////////////debug drawing///////////////////////
#endif
	if (bhitResult)
	{
		for (auto hitResult : hitResultArr)
		{
			ABaseCharacter* otherActor = dynamic_cast<ABaseCharacter*>(hitResult.GetActor());
			if (!otherActor) return;

			float damage = GetCharacterStatData().attackPower * 5;
			TSubclassOf<class UDamageType> types;

			FPointDamageEvent damageEvent(damage, hitResult, FVector::ZeroVector, types);

			float availableDamage = otherActor->TakeDamage(damage, damageEvent, GetController(), this);
		}
	}
}

void ABanditKing::MoveForward(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
	case AnimState::EQUIP:
	case AnimState::GUARD:
	case AnimState::PARRY:
	case AnimState::SKILLMODE:
	case AnimState::RUN:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), axisValue);
		break;
	default:
		break;

	}
}

void ABanditKing::MoveRight(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
	case AnimState::EQUIP:
	case AnimState::GUARD:
	case AnimState::PARRY:
	case AnimState::SKILLMODE:
	case AnimState::RUN:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), axisValue);
		break;
	default:
		break;
	}
}

void ABanditKing::LookUp(float axisValue)
{
	if (lockOnTarget) { return; }

	AddControllerPitchInput(axisValue);
}

void ABanditKing::Turn(float axisValue)
{
	if (lockOnTarget) { return; }

	AddControllerYawInput(axisValue);
}

void ABanditKing::Guard(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::EQUIP:
		if (axisValue && characterStat->GetCurrentStamina() >= 10.f)
		{
			GetWorldTimerManager().SetTimer(parryTimerHandle, this, &ABanditKing::ParryToGuard, 0.1f, false, 0.1f);
			currentAnimState = AnimState::PARRY;
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 6;
		}
		break;
	case AnimState::GUARD:
		if (!axisValue || characterStat->GetCurrentStamina() <= 0.f)
		{
			currentAnimState = AnimState::EQUIP;

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;
		}
		break;
	default:
		break;
	}
}

void ABanditKing::ParryToGuard()
{
	currentAnimState = AnimState::GUARD;
}


void ABanditKing::TakeStun()
{
	++stunStack;	


	if (stunStack >= 5)
	{
		currentAnimState = AnimState::STUN;
		banditKingAnim->PlayStunMontage();
		stunStack = 0;		
	}

	float divideStamina = GetCharacterStatData().stamina / 5;
	characterStat->SetCurrentStamina(divideStamina * (5 - stunStack));

}

void ABanditKing::SetDamage(float damage)
{
	characterStat->SetCurrentHp(characterStat->GetCurrentHp() - damage);

	float currentHp = characterStat->GetCurrentHp();

	if (!bSkillMode)
	{			
		if (currentHp < GetCharacterStatData().hp / 2)
		{
			banditKingAnim->PlayPhaseChangeMontage();
		}
	}



	if (currentHp <= 0.f)
	{
		characterStat->SetCurrentHp(0.f);

		currentAnimState = AnimState::DEAD;
		prevAnimState = AnimState::DEAD;

		weapon->SetCollision(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		GetMovementComponent()->StopActiveMovement();

		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

		SetActorEnableCollision(true);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;


		hpBarWidget->DestroyComponent();
		hpBarWidget = nullptr;

		weapon->SetWeaponParticle(false);
		weapon->SetCollision(false);
		banditKingAnim->Montage_Pause();
		
		GetWorldTimerManager().SetTimer(deadTimerHandle, this, &ABanditKing::Dead, 0.1f, false, 3.0f);
		
	}
}

void ABanditKing::Dead()
{
	currentAnimState = AnimState::DEAD;
	prevAnimState = AnimState::DEAD;

	auto playerController = dynamic_cast<AUserPlayerController*>(GetWorld()->GetFirstPlayerController());
	if (!playerController) return;

	playerController->AddGameCompleteWidget();
}

void ABanditKing::SetStaminaDecrease(float amount)
{
	bStaminaRegen = false;
	GetWorldTimerManager().SetTimer(staminaRegenerationTimerHandle, this, &ABanditKing::OnStaminaRegen, 0.1f, false, 1.5f);

	characterStat->SetCurrentStamina(characterStat->GetCurrentStamina() - amount);

	float currentStamina = characterStat->GetCurrentStamina();

	if (currentStamina <= 0.f)
		characterStat->SetCurrentStamina(0.f);
}

float ABanditKing::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	
	ABaseCharacter* enemyCharacter = dynamic_cast<ABaseCharacter*>(damageCauser);

	if (GetCharacterStatData().faction == enemyCharacter->GetCharacterStatData().faction)
		return 0;

	FVector vDir = GetActorLocation() - damageCauser->GetActorLocation();
	vDir = vDir.GetSafeNormal();


	FVector forwardVec = GetActorForwardVector();
	float radianForwardVec = FVector::DotProduct(forwardVec, vDir);
	float degreeX = FMath::RadiansToDegrees(acos(radianForwardVec));


	FVector rightVec = GetActorRightVector();
	float radianRightVec = FVector::DotProduct(rightVec, vDir);
	float degreeY = FMath::RadiansToDegrees(acos(radianRightVec));

	if (degreeY > 90.f)
	{
		degreeX *= -1.f;
	}

	FHitResult SweepResult;
	FVector dir;

	if (abs(degreeX) >= 100)
	{
		switch (currentAnimState)
		{
		case AnimState::GUARD:
		case AnimState::PARRY:
			banditKingAnim->PlayGuardMontage();
			weapon->OnEffect(SweepResult.Location);
			weapon->PlayGuardSound();
			SetStaminaDecrease(10.f);
			return 0;
			break;
		default:
			break;
		}
	}


	hpBarWidget->bVisible = true;
	BloodEffect(SweepResult.Location);
	SetDamage(damageAmount);
	return damageAmount;
}

void ABanditKing::BloodEffect(FVector hitPos)
{
	for (auto blood : bloodArr)
	{
		if (!blood->IsActive())
		{
			blood->SetRelativeLocationAndRotation(hitPos - GetActorLocation(), FRotator::ZeroRotator);
			blood->Activate();
			return;
		}
	}
}

void ABanditKing::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (banditKingAnim->Montage_IsPlaying(Montage))
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed;

	currentAnimState = prevAnimState;
	if (prevAnimState == AnimState::EQUIP || prevAnimState == AnimState::SKILLMODE)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;
	}
	currentCombo = 0;
	bInputCheck = false;
	weapon->SetCollision(false);
}

void ABanditKing::SetLockOnTarget(ABaseCharacter* otherCharacter)
{
	if (otherCharacter)
		lockOnTarget = otherCharacter;
}

void ABanditKing::LockOnTick()
{
	if (banditKingAnim->IsAnyMontagePlaying()) return;

	FVector vX(1.f, 0.f, 0.f);
	FVector vY(0.f, 1.f, 0.f);

	FVector vDir = lockOnTarget->GetActorLocation() - GetActorLocation();
	FVector vDirNormal = vDir.GetSafeNormal();
	float radiansX = FVector::DotProduct(vX, vDirNormal);
	float radiansY = FVector::DotProduct(vY, vDirNormal);

	float degreeX = FMath::RadiansToDegrees(acos(radiansX));
	float degreeY = FMath::RadiansToDegrees(acos(radiansY));
	if (degreeY > 90.f)
	{
		degreeX *= -1.f;
	}
	FRotator targetRotator = FRotationMatrix::MakeFromX(vDirNormal).Rotator();


	float pitchDegree = (vDir.Size() - 1001.f) / 50.f - 20.f;

	targetRotator.Pitch = pitchDegree;

	switch (currentAnimState)
	{
	case AnimState::GENERAL:
	case AnimState::EQUIP:
	case AnimState::GUARD:
	case AnimState::PARRY:
	case AnimState::PARRYING:
		GetCapsuleComponent()->SetRelativeRotation(FMath::RInterpTo(GetController()->GetControlRotation(),
			FRotator(0.f, targetRotator.Yaw, 0.f), GetWorld()->GetDeltaSeconds(), 3.f));
		break;
	}
}


void ABanditKing::OnStaminaRegen()
{
	bStaminaRegen = true;
}



AnimState ABanditKing::GetAnimState()
{
	return currentAnimState;;
}

const FCharacterDataTable& ABanditKing::GetCharacterStatData()
{
	auto gameInstance = Cast<UCharacterGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto defaultData = gameInstance->GetCharacterData(TEXT("BanditKing"));
	return *defaultData;
}

ABaseCharacter* ABanditKing::GetLockOnTarget()
{
	return lockOnTarget;
}