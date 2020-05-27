// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCharacter.h"
#include "HumanAnimInstance.h"
#include "TwoHandSword.h"
#include "OneHandSword.h"
#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "HumanAIController.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "TutorialPlayerController.h"
#include "CharacterStatComponent.h"
#include "CharacterGameInstance.h"
#include "CharacterAssetLoader.h"
#include "HumanCharacterWidget.h"
#include "UserPlayerController.h"
#include "KnightAIController.h"
#include "UserHUDWidget.h"
#include "MainGameState.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "CameraShakeEffect.h"

ATutorialCharacter::ATutorialCharacter() : lockOnTarget(nullptr), currentCombo(0)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	characterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("STAT"));

	attackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ATTACKSOUND"));
	electronicSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ELECTRONICSOUND"));
	footWalkSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FOOTWALKSOUND"));

	characterName = TEXT("Player");

	//Set Attachment
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(
		TEXT("/Game/Characters/GhostLady_S2/Meshes/Characters/Combines/SK_GhostLadyS2_B.SK_GhostLadyS2_B"));
	if (SK_BASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BASE.Object);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	springArm->TargetArmLength = 400.f;
	springArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	springArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel2;
	springArm->ProbeSize = 1.0f;
	springArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	   
	FName head_Socket(TEXT("head_Socket"));
	if (GetMesh()->DoesSocketExist(head_Socket))
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HEAD(
			TEXT("/Game/Characters/GhostLady_S2/Meshes/Characters/Separates/Hairs/SK_LongHair.SK_LongHair"));
		if (SK_HEAD.Succeeded())
		{
			head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEAD"));
			head->SetSkeletalMesh(SK_HEAD.Object);
			head->SetupAttachment(GetMesh(), head_Socket);
		}
	}


	static ConstructorHelpers::FClassFinder<UAnimInstance> HUMAN_ANIM(
		TEXT("/Game/Characters/GhostLady_S2/Animations/HumanAnimBP.HumanAnimBP_C"));
	if (HUMAN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HUMAN_ANIM.Class);
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_ELECTRONIC(
		TEXT("/Game/FXVarietyPack/Particles/P_ky_lightning3.P_ky_lightning3"));
	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("COUNTATTACK %d"), i);
		UParticleSystemComponent* electronicEffect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		electronicEffect->SetupAttachment(GetCapsuleComponent());
		electronicEffect->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
		electronicEffect->bAutoActivate = false;
		electronicEffect->SetTemplate(P_ELECTRONIC.Object);
		electronicEffect->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		electronicEffectArr.Add(electronicEffect);
	}


	static ConstructorHelpers::FObjectFinder<USoundCue> ATTACKSOUNDCUE(TEXT("/Game/Sound/A_RandSwordAttack.A_RandSwordAttack"));
	if (ATTACKSOUNDCUE.Succeeded())
	{
		attackSoundCue = ATTACKSOUNDCUE.Object;
	}

	attackSoundComponent->bAutoActivate = false;
	attackSoundComponent->SetupAttachment(GetCapsuleComponent());
	attackSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundCue> ELECTRONICSOUNDCUE(TEXT("/Game/DarkMagic/DarkMagicCue/CountAttackCue.CountAttackCue"));
	if (ELECTRONICSOUNDCUE.Succeeded())
	{
		electronicSoundCue = ELECTRONICSOUNDCUE.Object;
	}

	electronicSoundComponent->bAutoActivate = false;
	electronicSoundComponent->SetupAttachment(GetCapsuleComponent());
	electronicSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


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

	lockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LOCKONWIDGET"));
	lockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOCKON(TEXT("/Game/UI/UI_LockOn.UI_LockOn_C"));
	if (UI_LOCKON.Succeeded())
	{
		lockOnWidget->SetWidgetClass(UI_LOCKON.Class);
		lockOnWidget->SetDrawSize(FVector2D(50.f, 50.f));
		lockOnWidget->bVisible = false;
	}

	AIControllerClass = AHumanAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


// Called when the game starts or when spawned

void ATutorialCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	humanAnim = Cast<UHumanAnimInstance>(GetMesh()->GetAnimInstance());
	humanAnim->OnMontageEnded.AddDynamic(this, &ATutorialCharacter::OnMontageEnded);

	humanAnim->OnNextAttackCheck.AddLambda([this]() -> void {

		if (bInputCheck)
		{
			if (moveForwardValue == 0 && moveRightValue == 0 && playerController)
			{
				if (playerController->GetTutorialState() == TutorialState::InPlaceAttackTutorial)
				{
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);
				}
			}

			bInputCheck = false;
			weapon->SetCollision(true);
			humanAnim->JumpToAttackMontageSection(currentCombo);
		}
	});

	humanAnim->OnNewAttack.AddLambda([this]() -> void {
		weapon->SetCollision(true);
	});

	humanAnim->OnCountAttack.AddLambda([this]() -> void {

		if (bInputCheck)
		{
			if (playerController)
			{
				if (playerController->GetTutorialState() == TutorialState::ParryCountTutorial)
				{
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 1.5f);
				}
			}

			bInputCheck = false;
			humanAnim->JumpToCountAttackMontageSection();
			weapon->SetCollision(true);
		}
	});

	humanAnim->OnNextSkillAttack.AddLambda([this]() -> void {

		if (bInputCheck)
		{
			bInputCheck = false;
			weapon->SetCollision(true);
			SetStaminaDecrease(5.f);
			humanAnim->JumpToSkillAttackMontageSection(currentCombo);
			if (currentCombo >= 4)
			{
				if (playerController->GetTutorialState() == TutorialState::SkillAttackTutorial)
				{									
					lockOnWidget->bVisible = false;
					lockOnTarget = nullptr;
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 2.f);
				}
			}
		}
	});

	humanAnim->OnAttackCheck.AddLambda([this]() -> void {
		ElectronicAttack();
	});

	humanAnim->OnSoundPlay.AddLambda([this]() -> void {
		attackSoundComponent->Play();
	});

	humanAnim->OnFootWalkSound.AddLambda([this]() -> void {
		footWalkSoundComponent->Play();
	});

	if (attackSoundCue->IsValidLowLevelFast())
	{
		attackSoundComponent->SetSound(attackSoundCue);
	}

	if (attackSoundCue->IsValidLowLevelFast())
	{
		electronicSoundComponent->SetSound(electronicSoundCue);
	}

	if (footWalkSoundCue->IsValidLowLevelFast())
	{
		footWalkSoundComponent->SetSound(footWalkSoundCue);
	}
}

void ATutorialCharacter::BeginPlay()
{
	Super::BeginPlay();


	characterStat->InitData(characterName, hp);


	FName clavicle_rSocket(TEXT("clavicle_rSocket"));
	if (GetMesh()->DoesSocketExist(clavicle_rSocket))
	{
		weapon = GetWorld()->SpawnActor<ATwoHandSword>(ATwoHandSword::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, clavicle_rSocket);
		weapon->SetOwner(this);
	}





	auto characterWidget = Cast<UHumanCharacterWidget>(hpBarWidget->GetUserWidgetObject());

	if (characterWidget != nullptr)
	{
		characterWidget->BindCharacterWidget(characterStat);
	}

	auto controller = dynamic_cast<ATutorialPlayerController*>(GetController());
	if (controller)
		playerController = controller;
	else
		playerController = nullptr;

	if (characterName == TEXT("Bandit"))
	{
		Equip();
	}
}

// Called every framedd
void ATutorialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (lockOnTarget)
		LockOnTick();

	if (bHpRegen && characterStat->GetCurrentHp() + 1.f < GetCharacterStatData().hp && IsPlayerControlled())
		characterStat->SetCurrentHp(characterStat->GetCurrentHp() + 0.05f);

	if (bStaminaRegen && characterStat->GetCurrentStamina() + 1.f < GetCharacterStatData().stamina && IsPlayerControlled())
		characterStat->SetCurrentStamina(characterStat->GetCurrentStamina() + 0.1f);
}

// Called to bind functionality to input
void ATutorialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATutorialCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATutorialCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATutorialCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATutorialCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("Guard"), this, &ATutorialCharacter::Guard);
	PlayerInputComponent->BindAxis(TEXT("Run"), this, &ATutorialCharacter::Run);

	PlayerInputComponent->BindAction(TEXT("Equip"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::Equip);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::LockOn);
	PlayerInputComponent->BindAction(TEXT("OnSkillMode"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::SetSkillMode);
	PlayerInputComponent->BindAction(TEXT("SkillA"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::SkillA);
	PlayerInputComponent->BindAction(TEXT("SkillB"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::SkillB);

	//	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &ATutorialCharacter::Dodge);
}

void ATutorialCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

void ATutorialCharacter::MoveForward(float axisValue)
{
	if (!canInput) return;


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

	moveForwardValue = axisValue;
}

void ATutorialCharacter::MoveRight(float axisValue)
{
	if (!canInput) return;

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

	moveRightValue = axisValue;
}

void ATutorialCharacter::LookUp(float axisValue)
{
	if (lockOnTarget) { return; }

	AddControllerPitchInput(axisValue);
}

void ATutorialCharacter::Turn(float axisValue)
{
	if (lockOnTarget) { return; }

	AddControllerYawInput(axisValue);
}

void ATutorialCharacter::ShakeEffect(float scale)
{
	if (IsPlayerControlled())
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(UCameraShakeEffect::StaticClass(), scale);
}

void ATutorialCharacter::Run(float axisValue)
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

void ATutorialCharacter::Guard(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (axisValue && characterStat->GetCurrentStamina() > 0.f)
		{
			GetWorldTimerManager().SetTimer(parryTimerHandle, this, &ATutorialCharacter::ParryToGuard, 0.1f, false, 0.2f);
			currentAnimState = AnimState::PARRY;
			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 6;
		}
		break;
	case AnimState::GUARD:
		if (!axisValue || characterStat->GetCurrentStamina() <= 0.f)
		{
			if (prevAnimState == AnimState::SKILLMODE)
				currentAnimState = AnimState::SKILLMODE;
			else
				currentAnimState = AnimState::EQUIP;

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;
		}
		break;
	default:
		break;
	}


}

void ATutorialCharacter::ParryToGuard()
{
	UE_LOG(LogClass, Warning, TEXT("ParryEnd"));
	currentAnimState = AnimState::GUARD;
}

void ATutorialCharacter::Attack()
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
	{
		if (humanAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;

		if (playerController)
		{
			FVector vDir = GetVelocity();
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

			if (abs(degreeX) <= 85.f && moveRightValue == 0.f)
				if (playerController->GetTutorialState() == TutorialState::ForwardAttackTutorial)
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);
			
			if (abs(degreeX) >= 95.f && moveRightValue == 0.f)
				if (playerController->GetTutorialState() == TutorialState::PierceAttackTutorial)
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);			
		}


		humanAnim->PlayInPlaceAttackMontage();
		humanAnim->JumpToAttackMontageSection(currentCombo);

		weapon->SetCollision(true);
		break;
	}
	case AnimState::SKILLMODE:
		if (humanAnim->IsAnyMontagePlaying()) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		humanAnim->PlaySkillAttackMontage();
		humanAnim->JumpToAttackMontageSection(currentCombo);
		SetStaminaDecrease(5.f);
		weapon->SetCollision(true);
	case AnimState::RUN:
		if (humanAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentAnimState = AnimState::ATTACKING;
		humanAnim->PlayRunAttackMontage();
	default:
		break;
	}
}

void ATutorialCharacter::SkillA()
{
	if (characterStat->GetCurrentStamina() < 30.f)
		return;

	switch (currentAnimState)
	{
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (humanAnim->IsAnyMontagePlaying()) return;
		currentAnimState = AnimState::ATTACKING;
		if (playerController->GetTutorialState() == TutorialState::SkillATutorial)
		{
			GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);
		}
		humanAnim->PlaySkillAMontage();		
		SetStaminaDecrease(30.f);
	case AnimState::RUN:
		if (humanAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentAnimState = AnimState::ATTACKING;
		SetStaminaDecrease(30.f);
		humanAnim->PlaySkillARunMontage();
		if (playerController->GetTutorialState() == TutorialState::SkillATutorial)
		{
			GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);
		}
	default:
		break;
	}
}

void ATutorialCharacter::SkillB()
{
	if (characterStat->GetCurrentStamina() < 30.f)
		return;

	switch (currentAnimState)
	{
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (humanAnim->IsAnyMontagePlaying()) return;
		currentAnimState = AnimState::ATTACKING;
		humanAnim->PlaySkillBMontage();
		SetStaminaDecrease(30.f);

		if (playerController->GetTutorialState() == TutorialState::SkillBTutorial)
		{
			GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.5f);
		}
	default:
		break;
	}
}

void ATutorialCharacter::ElectronicAttack()
{
	FCollisionQueryParams Params(NAME_None, false, this);
	FHitResult hitResult;

	float range = 250.0;
	float radius = 150.f;

	ShakeEffect(10.f);
	electronicSoundComponent->Play();
	for (auto electronicEffect : electronicEffectArr)
	{
		if (!electronicEffect->IsActive())
		{
			electronicEffect->Activate();
			break;
		}
	}


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

			float damage = GetCharacterStatData().attackPower * 3;
			TSubclassOf<class UDamageType> types;

			FPointDamageEvent damageEvent(damage, hitResult, FVector::ZeroVector, types);

			float availableDamage = otherActor->TakeDamage(damage, damageEvent, GetController(), this);
		}
	}
}

void ATutorialCharacter::Equip()
{
	FName clavicle_rSocket(TEXT("clavicle_rSocket"));
	if (!(GetMesh()->DoesSocketExist(clavicle_rSocket)))
	{
		return;
	}

	FName hand_rSocket(TEXT("hand_rSocket"));
	if (!(GetMesh()->DoesSocketExist(hand_rSocket)))
	{
		return;
	}

	switch (currentAnimState)
	{
	case AnimState::GENERAL:
	{
		if (playerController)
		{
			if (playerController->GetTutorialState() == TutorialState::EquipTutorial)
				playerController->SetChangeTutorialState(TutorialState::LockOnTutorial);
		}

		weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, hand_rSocket);
		weapon->SetOwner(this);
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		currentAnimState = AnimState::EQUIP;
		prevAnimState = currentAnimState;
		break;
	}
	case AnimState::EQUIP:
		weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, clavicle_rSocket);
		weapon->SetOwner(this);
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		currentAnimState = AnimState::GENERAL;
		prevAnimState = currentAnimState;
		break;
	default:
		break;
	}
}

void ATutorialCharacter::SetSkillMode()
{
	switch (currentAnimState)
	{
		
	case AnimState::EQUIP:
		if (playerController->GetTutorialState() == TutorialState::SkillModeTutorial)
		{
			GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.1f);
		}
		currentAnimState = AnimState::SKILLMODE;
		prevAnimState = currentAnimState;
		weapon->SetWeaponParticle(true);
		break;
	case AnimState::SKILLMODE:
		currentAnimState = AnimState::EQUIP;
		prevAnimState = currentAnimState;
		weapon->SetWeaponParticle(false);
		break;
	default:
		break;
	}
}

void ATutorialCharacter::LockOn()
{
	if (lockOnTarget)
	{
		lockOnWidget->bVisible = false;
		lockOnTarget = nullptr;
		return;
	}

	FCollisionQueryParams Params(NAME_None, false, this);
	FHitResult hitResult;

	float range = 900.f;
	float radius = 600.f;



	TArray<FHitResult> hitResultArr;
	bool bhitResult = GetWorld()->SweepMultiByChannel(
		hitResultArr,
		GetActorLocation() + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * radius,
		GetActorLocation() + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * range,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(radius),
		Params);


#if DRAW_DEBUG
	///////////////////debug drawing///////////////////////
	FVector traceVec = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * range;
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
			if (!otherActor)
				return;

			if (!(otherActor->GetCharacterStatData().faction == GetCharacterStatData().faction))
			{
				lockOnTarget = dynamic_cast<ABaseCharacter*>(hitResult.GetActor());
				if (GetController()->IsPlayerController())
				{
					lockOnWidget->bVisible = true;					
				
					if (playerController->GetTutorialState() == TutorialState::LockOnTutorial)
						playerController->SetChangeTutorialState(TutorialState::InPlaceAttackTutorial);
					
		
				}
				return;
			}
		}
	}
}

void ATutorialCharacter::LockOnTick()
{
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


	if (!GetController()->IsPlayerController())
	{
		switch (currentAnimState)
		{
		case AnimState::GENERAL:
		case AnimState::EQUIP:
		case AnimState::GUARD:
		case AnimState::PARRY:
		case AnimState::PARRYING:
			GetCapsuleComponent()->SetRelativeRotation(FMath::RInterpTo(GetController()->GetControlRotation(),
				FRotator(0.f, targetRotator.Yaw, 0.f), GetWorld()->GetDeltaSeconds(), 1.f));
			break;
		}
	}
	else
		GetController()->SetControlRotation(FMath::RInterpTo(GetController()->GetControlRotation(), targetRotator, GetWorld()->GetDeltaSeconds(), 2.f));
	
	lockOnWidget->SetRelativeLocationAndRotation(lockOnTarget->GetActorLocation(), FRotator::ZeroRotator);
	if ((GetDistanceTo(lockOnTarget) > 2000) || (lockOnTarget->GetAnimState() == DEAD))
	{
		lockOnWidget->bVisible = false;
		lockOnTarget = nullptr;
	}
}


void ATutorialCharacter::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (humanAnim->Montage_IsPlaying(Montage))
	{
		return;
	}


	switch (currentAnimState)
	{
	case AnimState::GUARD:
		currentAnimState = AnimState::GUARD;
		return;
		break;
	default:
		break;
	}


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

void ATutorialCharacter::SetDamage(float damage)
{
	characterStat->SetCurrentHp(characterStat->GetCurrentHp() - damage);

	float currentHp = characterStat->GetCurrentHp();


	bHpRegen = false;
	GetWorldTimerManager().SetTimer(hpRegenerationTimerHandle, this, &ATutorialCharacter::OnHpRegen, 0.1f, false, 5.f);

	bStaminaRegen = false;
	GetWorldTimerManager().SetTimer(staminaRegenerationTimerHandle, this, &ATutorialCharacter::OnStaminaRegen, 0.1f, false, 2.f);
}

void ATutorialCharacter::SetStaminaDecrease(float amount)
{
	bHpRegen = false;
	GetWorldTimerManager().SetTimer(hpRegenerationTimerHandle, this, &ATutorialCharacter::OnHpRegen, 0.1f, false, 5.f);

	bStaminaRegen = false;
	GetWorldTimerManager().SetTimer(staminaRegenerationTimerHandle, this, &ATutorialCharacter::OnStaminaRegen, 0.1f, false, 1.5f);

	characterStat->SetCurrentStamina(characterStat->GetCurrentStamina() - amount);

	float currentStamina = characterStat->GetCurrentStamina();

	if (currentStamina <= 0.f)
	{
		float remainDamage = abs(currentStamina);
		SetDamage(remainDamage);
		characterStat->SetCurrentStamina(0.f);
	}
}

void ATutorialCharacter::BloodEffect(FVector hitPos)
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

void ATutorialCharacter::TakeStun()
{
	currentAnimState = AnimState::STUN;
	weapon->SetCollision(false);
	humanAnim->PlayStunMontage();
}

float ATutorialCharacter::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	if (currentAnimState == AnimState::DODGE) return 0;


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
	damageEvent.GetBestHitInfo(damageCauser, eventInstigator, SweepResult, dir);

	ShakeEffect(1.f);

	if (abs(degreeX) >= 100)
	{
		switch (currentAnimState)
		{
		case AnimState::GUARD:
		{
			if (playerController)
			{
				if (playerController->GetTutorialState() == TutorialState::GuardTutorial)
				{
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.7f);
				}
			}
			humanAnim->PlayGuardMontage();
			weapon->OnEffect(SweepResult.Location);
			weapon->PlayGuardSound();
			SetStaminaDecrease(damageAmount);
			return 0;
			break;
		}
		case AnimState::PARRY:
		{
			if (playerController)
			{
				if (playerController->GetTutorialState() == TutorialState::ParryTutorial)
				{
					GetWorldTimerManager().SetTimer(tutorialTimerHandle, this, &ATutorialCharacter::TutorialUpdate, 0.1f, false, 0.7f);
				}
			}

			currentAnimState = AnimState::PARRYING;
			humanAnim->PlayParryMontage();
			weapon->OnEffect(SweepResult.Location);
			weapon->PlayParrySound();
			enemyCharacter->TakeStun();
			return 0;
			break;
		}
		default:
			break;
		}
	}

	currentAnimState = AnimState::HIT;
	weapon->SetCollision(false);

	if (degreeX > 80.f && degreeX < 100.f)
	{//left
		humanAnim->PlayLeftHitMontage();
	}
	else if (abs(degreeX) >= 100.f)
	{//front
		humanAnim->PlayFwdHitMontage();
	}
	else if (degreeX > -100.f && degreeX < -80.f)
	{//right
		humanAnim->PlayRightHitMontage();
	}
	else if (abs(degreeX) <= 100.f)
	{//back
		humanAnim->PlayBwdHitMontage();
	}

	BloodEffect(SweepResult.Location);


	if (enemyCharacter->GetController()->IsPlayerController())
	{
		hpBarWidget->bVisible = true;
		GetWorldTimerManager().SetTimer(hpBarTimerHandle, this, &ATutorialCharacter::OffHpBarWidget, 0.1f, false, 8.0f);
	}

	SetDamage(damageAmount);


	return damageAmount;
}

void ATutorialCharacter::OffHpBarWidget()
{
	hpBarWidget->bVisible = false;
}

void ATutorialCharacter::TutorialUpdate()
{
	switch (playerController->GetTutorialState())
	{
	case TutorialState::MoveTutorial:
		playerController->SetChangeTutorialState(TutorialState::EquipTutorial);
		break;
	case TutorialState::EquipTutorial:
		playerController->SetChangeTutorialState(TutorialState::LockOnTutorial);
		break;
	case TutorialState::LockOnTutorial:
		playerController->SetChangeTutorialState(TutorialState::InPlaceAttackTutorial);
		break;
	case TutorialState::InPlaceAttackTutorial:
		playerController->SetChangeTutorialState(TutorialState::ForwardAttackTutorial);
		break;
	case TutorialState::ForwardAttackTutorial:
		playerController->SetChangeTutorialState(TutorialState::PierceAttackTutorial);
		break;
	case TutorialState::PierceAttackTutorial:
		playerController->SetChangeTutorialState(TutorialState::SkillATutorial);
		break;
	case TutorialState::SkillATutorial:
		playerController->SetChangeTutorialState(TutorialState::SkillBTutorial);
		break;
	case TutorialState::SkillBTutorial:
		playerController->SetChangeTutorialState(TutorialState::SkillModeTutorial);
		break;
	case TutorialState::SkillModeTutorial:
		playerController->SetChangeTutorialState(TutorialState::SkillAttackTutorial);
		break;
	case TutorialState::SkillAttackTutorial:
		playerController->SetChangeTutorialState(TutorialState::MoveToGuardTutorial);
		break;
	case TutorialState::MoveToGuardTutorial:
		playerController->SetChangeTutorialState(TutorialState::GuardTutorial);
		break;
	case TutorialState::GuardTutorial:
		playerController->SetChangeTutorialState(TutorialState::ParryTutorial);
		break;
	case TutorialState::ParryTutorial:
		playerController->SetChangeTutorialState(TutorialState::ParryCountTutorial);
		break;
	case TutorialState::ParryCountTutorial:
		playerController->SetChangeTutorialState(TutorialState::CompleteTutorial);
		break;
	default:
		break;
	}

}


void ATutorialCharacter::SetCanMove(bool flag)
{
	canInput = flag;
}

void ATutorialCharacter::SetLockOnTarget(ABaseCharacter* otherCharacter)
{
	if (otherCharacter)
		lockOnTarget = otherCharacter;
}

void ATutorialCharacter::OnHpRegen()
{
	bHpRegen = true;
}

void ATutorialCharacter::OnStaminaRegen()
{
	bStaminaRegen = true;
}

AnimState ATutorialCharacter::GetAnimState()
{
	return currentAnimState;
}

ABaseCharacter* ATutorialCharacter::GetLockOnTarget()
{
	return lockOnTarget;
}

const FCharacterDataTable& ATutorialCharacter::GetCharacterStatData()
{
	return characterStat->GetCharacterStatData();
}