// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanCharacter.h"
#include "HumanAnimInstance.h"
#include "TwoHandSword.h"
#include "OneHandSword.h"
#include "Weapon.h"
#include "DrawDebugHelpers.h"
#include "HumanAIController.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
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

// Sets default values
AHumanCharacter::AHumanCharacter() : lockOnTarget(nullptr), currentCombo(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	characterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("STAT"));
	
	attackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ATTACKSOUND"));
	electronicSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ELECTRONICSOUND"));
	footWalkSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FOOTWALKSOUND"));
	
	characterName = TEXT("Bandit");	

	//Set Attachment
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(
		TEXT("/Game/Characters/GhostLady_S2/Meshes/Characters/Combines/SK_GhostLadyS2_A.SK_GhostLadyS2_A"));
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
		head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEAD"));
		head->SetSkeletalMesh(nullptr);
		head->SetupAttachment(GetMesh(), head_Socket);
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
}


// Called when the game starts or when spawned

void AHumanCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	humanAnim = Cast<UHumanAnimInstance>(GetMesh()->GetAnimInstance());
	humanAnim->OnMontageEnded.AddDynamic(this, &AHumanCharacter::OnMontageEnded);
	
	humanAnim->OnNextAttackCheck.AddLambda([this]() -> void {
				
		if (bInputCheck)
		{
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

void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();   
	
	characterStat->InitData(characterName, 100.f);

	auto defaultAsset = GetDefault<UCharacterAssetLoader>();
	
	characterAssetToLoad = defaultAsset->characterAssets[GetCharacterStatData().mainAssetNumber];
	auto characterGameInstance = Cast<UCharacterGameInstance>(GetGameInstance());
	if (nullptr != characterGameInstance)
	{
		assetStreamingHandle = characterGameInstance->StreamableManager.RequestAsyncLoad(characterAssetToLoad,
			FStreamableDelegate::CreateUObject(this, &AHumanCharacter::OnAssetLoadCompleted));		
	}

	if (GetCharacterStatData().headAssetNumber >= 0)
	{
		characterHeadAssetToLoad = defaultAsset->characterAssets[GetCharacterStatData().headAssetNumber];
		if (nullptr != characterGameInstance)
		{
			if (GetCharacterStatData().headAssetNumber == 5)
			{
				FName head_Socket(TEXT("head_Socket"));
				if (GetMesh()->DoesSocketExist(head_Socket))
				{
					head->SetRelativeLocationAndRotation(FVector(-0.245437f, -2.147278f, 166.553497f), FRotator::ZeroRotator);
				}
			}
			assetStreamingHandle = characterGameInstance->StreamableManager.RequestAsyncLoad(characterHeadAssetToLoad,
				FStreamableDelegate::CreateUObject(this, &AHumanCharacter::OnHeadAssetLoadCompleted));
		}
	}	

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
}

// Called every framedd
void AHumanCharacter::Tick(float DeltaTime)
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
void AHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHumanCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHumanCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHumanCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHumanCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("Guard"), this, &AHumanCharacter::Guard);
	PlayerInputComponent->BindAxis(TEXT("Run"), this, &AHumanCharacter::Run);

	PlayerInputComponent->BindAction(TEXT("Equip"), EInputEvent::IE_Pressed, this, &AHumanCharacter::Equip);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AHumanCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &AHumanCharacter::LockOn);
	PlayerInputComponent->BindAction(TEXT("OnSkillMode"), EInputEvent::IE_Pressed, this, &AHumanCharacter::SetSkillMode);
	PlayerInputComponent->BindAction(TEXT("SkillA"), EInputEvent::IE_Pressed, this, &AHumanCharacter::SkillA);
	PlayerInputComponent->BindAction(TEXT("SkillB"), EInputEvent::IE_Pressed, this, &AHumanCharacter::SkillB);

//	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AHumanCharacter::Dodge);
}

void AHumanCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
}

void AHumanCharacter::MoveForward(float axisValue)
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
}

void AHumanCharacter::MoveRight(float axisValue)
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
}

void AHumanCharacter::LookUp(float axisValue)
{
	if (lockOnTarget) { return; }

	AddControllerPitchInput(axisValue);
}

void AHumanCharacter::Turn(float axisValue)
{
	if (lockOnTarget)
	{
		if (axisValue == 0)
		{
			lockOnChangeWeight = 0;
			return;
		}	

		lockOnChangeWeight += axisValue;
		if (abs(lockOnChangeWeight) > 15)
		{
			LockOnChange(lockOnChangeWeight);
			lockOnChangeWeight = 0;	
		}		
		
		return;
	}

	AddControllerYawInput(axisValue);
}


void AHumanCharacter::Dodge()
{
	switch (currentAnimState)
	{
	case AnimState::EQUIP:
		if (GetVelocity().Size() && characterStat->GetCurrentStamina() >= 20.f)
		{
			if (humanAnim->PlayDodgeMontage())
			{
				currentAnimState = AnimState::DODGE;
				SetStaminaDecrease(20.f);
			}
		}
		break;
	default:
		break;
	}
}

void AHumanCharacter::Run(float axisValue)
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

void AHumanCharacter::Guard(float axisValue)
{	
	switch (currentAnimState)
	{	
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (axisValue && characterStat->GetCurrentStamina() > 0.f)
		{	
			GetWorldTimerManager().SetTimer(parryTimerHandle, this, &AHumanCharacter::ParryToGuard, 0.1f, false, 0.2f);
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

void AHumanCharacter::ParryToGuard()
{
	UE_LOG(LogClass, Warning, TEXT("ParryEnd"));
	currentAnimState = AnimState::GUARD;
}

void AHumanCharacter::Attack()
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
		if (humanAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentCombo = 1;
		bInputCheck = false;
		currentAnimState = AnimState::ATTACKING;
		humanAnim->PlayInPlaceAttackMontage();
		humanAnim->JumpToAttackMontageSection(currentCombo);

		weapon->SetCollision(true);
		break;
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

void AHumanCharacter::SkillA()
{
	if (characterStat->GetCurrentStamina() < 30.f)
		return;	

	switch (currentAnimState)
	{
	case AnimState::EQUIP:
	case AnimState::SKILLMODE:
		if (humanAnim->IsAnyMontagePlaying()) return;
		currentAnimState = AnimState::ATTACKING;
		humanAnim->PlaySkillAMontage();
		SetStaminaDecrease(30.f);
	case AnimState::RUN:
		if (humanAnim->IsAnyMontagePlaying() || (prevAnimState == AnimState::GENERAL)) return;
		currentAnimState = AnimState::ATTACKING;
		SetStaminaDecrease(30.f);
		humanAnim->PlaySkillARunMontage();
	default:
		break;
	}
}

void AHumanCharacter::SkillB()
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
	default:
		break;
	}
}

void AHumanCharacter::ElectronicAttack()
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

void AHumanCharacter::Equip()
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
		weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, hand_rSocket);
		weapon->SetOwner(this);
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStatData().maxWalkSpeed / 2;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		currentAnimState = AnimState::EQUIP;	
		prevAnimState = currentAnimState;
		break;
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

void AHumanCharacter::SetSkillMode()
{
	switch (currentAnimState)
	{
	case AnimState::EQUIP:
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

void AHumanCharacter::LockOn()
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
	if (!bhitResult) return;
		
	for (auto hitResult : hitResultArr)
	{
		ABaseCharacter* otherActor = dynamic_cast<ABaseCharacter*>(hitResult.GetActor());
		if (!otherActor) continue;
		if (otherActor->GetAnimState() == AnimState::DEAD) continue;
		if (!(otherActor->GetCharacterStatData().faction == GetCharacterStatData().faction))
		{
			lockOnTarget = otherActor;
			if (GetController()->IsPlayerController())
				lockOnWidget->bVisible = true;
			return;
		}
	}
	
}

void AHumanCharacter::LockOnChange(float axisValue)
{

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
	if (!bhitResult) return;
	
	float minDegree = 180.f;
	ABaseCharacter* minDegreeCharacter = nullptr;

	for (auto hitResult : hitResultArr)
	{
		ABaseCharacter* otherActor = dynamic_cast<ABaseCharacter*>(hitResult.GetActor());
		if (!otherActor) continue;
		if ((otherActor == lockOnTarget) || (otherActor->GetAnimState() == AnimState::DEAD)) continue;		

		FVector vDir = otherActor->GetActorLocation() - GetActorLocation();
		FVector vDirNormal = vDir.GetSafeNormal();
		float radian = FVector::DotProduct(GetActorRightVector(), vDirNormal);
		float degree = FMath::RadiansToDegrees(acos(radian));

		if (axisValue > 1 && degree >= 90.f) continue;
		else if (axisValue < -1 && degree <= 90) continue;		

		degree = abs(90 - degree);

		if (!(otherActor->GetCharacterStatData().faction == GetCharacterStatData().faction))
		{
			if (minDegree > degree)
			{
				minDegreeCharacter = otherActor;
				minDegree = degree;
			}
		}
	}

	if (minDegreeCharacter)
		lockOnTarget = minDegreeCharacter;	
}

void AHumanCharacter::LockOnTick()
{
	FVector vDir = lockOnTarget->GetActorLocation() - GetActorLocation();
	FVector vDirNormal = vDir.GetSafeNormal();

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

	lockOnWidget->SetRelativeLocation(lockOnTarget->GetActorLocation());

	if ((GetDistanceTo(lockOnTarget) > 2000) || (lockOnTarget->GetAnimState() == DEAD))
	{	
		lockOnWidget->bVisible = false;
		lockOnTarget = nullptr;
	}
}


void AHumanCharacter::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
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

void AHumanCharacter::SetDamage(float damage)
{	
	characterStat->SetCurrentHp(characterStat->GetCurrentHp() - damage);	

	float currentHp = characterStat->GetCurrentHp();


	bHpRegen = false;
	GetWorldTimerManager().SetTimer(hpRegenerationTimerHandle, this, &AHumanCharacter::OnHpRegen, 0.1f, false, 5.f);

	bStaminaRegen = false;
	GetWorldTimerManager().SetTimer(staminaRegenerationTimerHandle, this, &AHumanCharacter::OnStaminaRegen, 0.1f, false, 2.f);

	if (currentHp <= 0.f)
	{		
		characterStat->SetCurrentHp(0.f);

		
		if (!IsPlayerControlled())
		{
			GetWorldTimerManager().SetTimer(deadTimerHandle, this, &AHumanCharacter::DecreaseMainGameFactionCount, 0.1f, false, 3.0f);				
		}
		else
		{
			GetWorldTimerManager().SetTimer(userDeadTimerHandle, this, &AHumanCharacter::AddDeadWidget, 0.1f, false, 3.0f);
		}
		humanAnim->Montage_Pause();
		currentAnimState = AnimState::DEAD;
		prevAnimState = AnimState::DEAD;

		FName handSocket;


		weapon->SetCollision(false);
		
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	//	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	//	GetCapsuleComponent()->SetEnableGravity(true);		
		GetMovementComponent()->StopActiveMovement();	
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		
		//SetActorEnableCollision(true);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;


		if (hpBarWidget)
		{
			hpBarWidget->DestroyComponent();
			hpBarWidget = nullptr;
		}

		GetCapsuleComponent()->SetEnableGravity(false);
	}
}

void AHumanCharacter::SetStaminaDecrease(float amount)
{
	bHpRegen = false;
	GetWorldTimerManager().SetTimer(hpRegenerationTimerHandle, this, &AHumanCharacter::OnHpRegen, 0.1f, false, 5.f);

	bStaminaRegen = false;
	GetWorldTimerManager().SetTimer(staminaRegenerationTimerHandle, this, &AHumanCharacter::OnStaminaRegen, 0.1f, false, 1.5f);

	characterStat->SetCurrentStamina(characterStat->GetCurrentStamina() - amount);

	float currentStamina = characterStat->GetCurrentStamina();

	if (currentStamina <= 0.f)
	{	
		float remainDamage = abs(currentStamina);
		SetDamage(remainDamage);
		characterStat->SetCurrentStamina(0.f);
	}
}

void AHumanCharacter::BloodEffect(FVector hitPos)
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

void AHumanCharacter::TakeStun()
{
	currentAnimState = AnimState::STUN;
	weapon->SetCollision(false);
	humanAnim->PlayStunMontage();
}

float AHumanCharacter::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);
	
	if (currentAnimState == AnimState::DODGE || currentAnimState == AnimState::DEAD) return 0;


	ABaseCharacter* enemyCharacter = dynamic_cast<ABaseCharacter*>(damageCauser);

	if (GetCharacterStatData().faction== enemyCharacter->GetCharacterStatData().faction)
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
			humanAnim->PlayGuardMontage();
			weapon->OnEffect(SweepResult.Location);
			weapon->PlayGuardSound();
			SetStaminaDecrease(damageAmount);
			return 0;
			break;
		case AnimState::PARRY:
			currentAnimState = AnimState::PARRYING;
			humanAnim->PlayParryMontage();			
			weapon->OnEffect(SweepResult.Location);
			weapon->PlayParrySound();
			enemyCharacter->TakeStun();
			return 0;
			break;
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
	}

	SetDamage(damageAmount);	

	if (!GetController()->IsPlayerController())
	{
		if (enemyCharacter)		
			lockOnTarget = enemyCharacter;
	}

	return damageAmount;
}

void AHumanCharacter::DecreaseMainGameFactionCount()
{
	dynamic_cast<AMainGameState*>(GetWorld()->GetGameState())->DecreaseFactionCharacterCount(GetCharacterStatData().faction);
	currentAnimState = AnimState::DEAD;
	prevAnimState = AnimState::DEAD;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
}

void AHumanCharacter::ShakeEffect(float scale)
{
	if (IsPlayerControlled())
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(UCameraShakeEffect::StaticClass(), scale);
}

void AHumanCharacter::AddDeadWidget()
{
	auto playerController = dynamic_cast<AUserPlayerController*>(GetController());
	if (!playerController) return;

	playerController->AddDeadWidget();
}

void AHumanCharacter::SetCanMove(bool flag)
{
	canInput = flag;
}

void AHumanCharacter::InitCharacter(FName characterName, float hp)
{
	this->characterName = characterName;

	characterStat->InitData(this->characterName, hp);

	auto defaultAsset = GetDefault<UCharacterAssetLoader>();

	characterAssetToLoad = defaultAsset->characterAssets[GetCharacterStatData().mainAssetNumber];
	auto characterGameInstance = Cast<UCharacterGameInstance>(GetGameInstance());
	if (nullptr != characterGameInstance)
	{
		assetStreamingHandle = characterGameInstance->StreamableManager.RequestAsyncLoad(characterAssetToLoad,
			FStreamableDelegate::CreateUObject(this, &AHumanCharacter::OnAssetLoadCompleted));
	}

	if (GetCharacterStatData().headAssetNumber >= 0)
	{
		characterHeadAssetToLoad = defaultAsset->characterAssets[GetCharacterStatData().headAssetNumber];
		if (nullptr != characterGameInstance)
		{
			if (GetCharacterStatData().headAssetNumber == 5)
			{
				FName head_Socket(TEXT("head_Socket"));
				if (GetMesh()->DoesSocketExist(head_Socket))
				{					
					head->SetRelativeLocationAndRotation(FVector(-0.245437f, -2.147278f, 166.553497f), FRotator::ZeroRotator);
				}
			}
			assetStreamingHandle = characterGameInstance->StreamableManager.RequestAsyncLoad(characterHeadAssetToLoad,
				FStreamableDelegate::CreateUObject(this, &AHumanCharacter::OnHeadAssetLoadCompleted));
		}
	}
	

	if (characterName.ToString() == TEXT("Player"))
	{
		auto playerController = dynamic_cast<AUserPlayerController*>(GetWorld()->GetFirstPlayerController());
		auto temp = playerController->GetPawn();
		if (playerController)
		{
			PossessedBy(GetWorld()->GetFirstPlayerController());
			GetController()->Possess(this);
			temp->Destroy();		
		}
	}
	else
	{	
		if (characterName == TEXT("Bandit"))
			AIControllerClass = AHumanAIController::StaticClass();			
		else if (characterName == TEXT("Knight"))
			AIControllerClass = AKnightAIController::StaticClass();

		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		SpawnDefaultController();

		Equip();
	}

	SetDamage(0.f);
}

void AHumanCharacter::SetLockOnTarget(ABaseCharacter* otherCharacter)
{
	if (otherCharacter)
		lockOnTarget = otherCharacter;
}

void AHumanCharacter::OnHpRegen()
{
	bHpRegen = true;
}

void AHumanCharacter::OnStaminaRegen()
{
	bStaminaRegen = true;
}

AnimState AHumanCharacter::GetAnimState()
{
	return currentAnimState;
}

ABaseCharacter* AHumanCharacter::GetLockOnTarget()
{
	return lockOnTarget;
}

const FCharacterDataTable& AHumanCharacter::GetCharacterStatData()
{
	return characterStat->GetCharacterStatData();
}


bool AHumanCharacter::GetWritePermit()
{
	return bWrite;
}

void AHumanCharacter::OnAssetLoadCompleted()
{
	assetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> loadedAssetPath(characterAssetToLoad);	
	GetMesh()->SetSkeletalMesh(loadedAssetPath.Get());
}

void AHumanCharacter::OnHeadAssetLoadCompleted()
{
	assetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> loadedAssetPath(characterHeadAssetToLoad);
	head->SetSkeletalMesh(loadedAssetPath.Get());
}

