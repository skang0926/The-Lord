// Fill out your copyright notice in the Description page of Project Settings.


#include "Grux.h"
#include "GruxAnimInstance.h"
#include "HumanCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "HumanCharacterWidget.h"
#include "GruxAIController.h"
#include "CharacterStatComponent.h"
#include "CharacterGameInstance.h"
#include "Sound/SoundCue.h"
// Sets default values
AGrux::AGrux()
{
	maxHp = 200.f;
	currentHp = maxHp;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	raseSound = CreateDefaultSubobject<UAudioComponent>(TEXT("RASESOUND"));
	
	characterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("STAT"));


	hitSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HITSWORDSOUND"));
	guardSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GUARDSOUND"));
	parrySoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PARRYSOUND"));
	attackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ATTACKSOUND"));


	characterName = TEXT("Grux");	

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_BLOOD(
		TEXT("/Game/Effect/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact"));
	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("BLOOD %d"), i);
		UParticleSystemComponent* blood = CreateDefaultSubobject<UParticleSystemComponent>(name);
		blood->SetupAttachment(GetCapsuleComponent());
		blood->SetRelativeLocation(FVector(0.0f, -70.0f, 0.0f));
		blood->bAutoActivate = false;
		blood->SetTemplate(P_BLOOD.Object);
		blood->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
		bloodArr.Add(blood);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_EFFECT(TEXT("/Game/CloseCombat/Effects/Impacts/P_Imp_Metal.P_Imp_Metal"));

	for (int32 i = 0; i < 10; i++)
	{
		FName name = *FString::Printf(TEXT("EFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetupAttachment(GetCapsuleComponent());
		effect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_EFFECT.Object);

		effectComponentArr.Add(effect);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> HITSOUNDCUE(TEXT("/Game/Sound/A_GruxHitCue.A_GruxHitCue"));
	if (HITSOUNDCUE.Succeeded())
	{
		hitSoundCue = HITSOUNDCUE.Object;
	}

	hitSoundComponent->bAutoActivate = false;
	hitSoundComponent->SetupAttachment(RootComponent);
	hitSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> GUARDSOUNDCUE(TEXT("/Game/Sound/A_GuardCue.A_GuardCue"));
	if (GUARDSOUNDCUE.Succeeded())
	{
		guardSoundCue = GUARDSOUNDCUE.Object;
	}

	guardSoundComponent->bAutoActivate = false;
	guardSoundComponent->SetupAttachment(RootComponent);
	guardSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> PARRYSOUNDCUE(TEXT("/Game/Sound/A_ParryCue.A_ParryCue"));
	if (GUARDSOUNDCUE.Succeeded())
	{
		parrySoundCue = PARRYSOUNDCUE.Object;
	}

	parrySoundComponent->bAutoActivate = false;
	parrySoundComponent->SetupAttachment(RootComponent);
	parrySoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> ATTACKSOUNDCUE(TEXT("/Game/Sound/A_GruxAttackSoundCue.A_GruxAttackSoundCue"));
	if (ATTACKSOUNDCUE.Succeeded())
	{
		attackSoundCue = ATTACKSOUNDCUE.Object;
	}

	attackSoundComponent->bAutoActivate = false;
	attackSoundComponent->SetupAttachment(RootComponent);
	attackSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	//Set Attachment
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);
	

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.0f, 0.f));



	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SK_BASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BASE.Object);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	springArm->TargetArmLength = 400.f;
	springArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	springArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;
	
	  

	static ConstructorHelpers::FObjectFinder<USoundBase>A_RASESOUND(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/Sounds/SoundWaves/Grux_Ability_RMB_Engage_020.Grux_Ability_RMB_Engage_020"));
	if (A_RASESOUND.Succeeded())
	{			
		raseSound->SetupAttachment(GetCapsuleComponent());
		raseSound->bAutoActivate = false;
		raseSound->SetSound(A_RASESOUND.Object);
	}


	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> GRUX_ANIM(
		TEXT("/Game/Characters/ParagonGrux/Characters/Heroes/Grux/GruxAnimBP.GruxAnimBP_C"));
	if (GRUX_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GRUX_ANIM.Class);
	}


	hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	hpBarWidget->SetupAttachment(GetMesh());

	hpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		hpBarWidget->SetWidgetClass(UI_HUD.Class);
		hpBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
		hpBarWidget->bVisible = false;
	}

	AIControllerClass = AGruxAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AGrux::BeginPlay()
{
	Super::BeginPlay();
	
	characterStat->InitData(characterName, 200.f);

	auto characterWidget = Cast<UHumanCharacterWidget>(hpBarWidget->GetUserWidgetObject());

	UE_LOG(LogClass, Warning, TEXT("character Widget Null?"));
	if (characterWidget != nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("character Widget IsValid"));
		characterWidget->BindCharacterWidget(characterStat);
	}
}

// Called every frame
void AGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGrux::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGrux::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGrux::MoveRight);
	
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AGrux::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AGrux::Turn);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AGrux::Attack);
}

void AGrux::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gruxAnim = Cast<UGruxAnimInstance>(GetMesh()->GetAnimInstance());
	gruxAnim->OnMontageEnded.AddDynamic(this, &AGrux::OnMontageEnded);

	gruxAnim->OnAttackCheck.AddUObject(this, &AGrux::AttackCheck);

	if (hitSoundCue->IsValidLowLevelFast())
	{
		hitSoundComponent->SetSound(hitSoundCue);
	}

	if (guardSoundCue->IsValidLowLevelFast())
	{
		guardSoundComponent->SetSound(guardSoundCue);
	}

	if (parrySoundCue->IsValidLowLevelFast())
	{
		parrySoundComponent->SetSound(parrySoundCue);
	}

	if (attackSoundCue->IsValidLowLevelFast())
	{
		attackSoundComponent->SetSound(attackSoundCue);
	}
}

void AGrux::MoveForward(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), axisValue);
		break;
	}

	//AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), axisValue);	
}

void AGrux::MoveRight(float axisValue)
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), axisValue);
		break;
	}

	//AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), axisValue);
}

void AGrux::Attack()
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
		currentAnimState = AnimState::ATTACKING;
		gruxAnim->PlayAttackMontage();
		break;
	}
	
//	GruxAnim->PlayAttackMontage();
}

void AGrux::RaseAttack()
{
	switch (currentAnimState)
	{
	case AnimState::GENERAL:
		currentAnimState = AnimState::ATTACKING;
		PlayRaseSound();
		gruxAnim->PlayRaseAttackMontage();
		break;
	}
}

void AGrux::LookUp(float axisValue)
{
	AddControllerPitchInput(axisValue);
}


void AGrux::Turn(float axisValue)
{
	AddControllerYawInput(axisValue);
}


void AGrux::OnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (currentAnimState == AnimState::DEAD) return;

	currentAnimState = AnimState::GENERAL;
}

void AGrux::AttackCheck()
{
	attackSoundComponent->Play();
	FCollisionQueryParams Params(NAME_None, false, this);
	FHitResult hitResult;

	float range = 250.f;
	float radius = 100.f;

	bool bhitResult = GetWorld()->SweepSingleByChannel(
		hitResult,
		GetActorLocation() + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * radius,
		GetActorLocation() + GetActorForwardVector() * range,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(radius),
		Params);

	GetActorForwardVector();

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
	auto otherActor = dynamic_cast<ABaseCharacter*>(hitResult.GetActor());
		
	if (!otherActor || !bhitResult) return;
	
	float damage = 10.0f;
	TSubclassOf<class UDamageType> types;
	
	FPointDamageEvent damageEvent(damage, hitResult, FVector::ZeroVector, types);
	
	float availableDamage = otherActor->TakeDamage(damage, damageEvent, GetController(), this);
	
	if (availableDamage == -1)
	{ // otherActor's state is Guard
		guardSoundComponent->Play();
	}
	else if (availableDamage == -2)
	{ // ohterActor's state is Parry
		parrySoundComponent->Play();
	}
	else if (availableDamage == 0)
		return;
	else
	{
		hitSoundComponent->Play();
		return;
	}
	
	for (auto effect : effectComponentArr)
	{
		if (!effect->IsActive())
		{				
			effect->SetRelativeLocationAndRotation(hitResult.Location - otherActor->GetActorLocation(), FRotator::ZeroRotator);
			effect->Activate();			
			return;					
		}
	}
}

void AGrux::BloodEffect(FVector hitPos)
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

void AGrux::PlayRaseSound()
{
	raseSound->Activate();	
}

void AGrux::SetDamage(float damage)
{
	characterStat->SetCurrentHp(characterStat->GetCurrentHp() - damage);

	float currentHp = characterStat->GetCurrentHp();

	hpBarWidget->bVisible = true;

	if (currentHp <= 0.f)
	{
		characterStat->SetCurrentHp(0.f);
		currentAnimState = AnimState::DEAD;
		
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		GetMovementComponent()->StopActiveMovement();

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		gruxAnim->SetDead();

		hpBarWidget->DestroyComponent();
		hpBarWidget = nullptr;
	}
}


float AGrux::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	FHitResult SweepResult;
	FVector dir;
	damageEvent.GetBestHitInfo(damageCauser, eventInstigator, SweepResult, dir);

	BloodEffect(SweepResult.Location);

	SetDamage(damageAmount);

	return damageAmount;
}

AnimState AGrux::GetAnimState()
{
	return currentAnimState;
}


const struct FCharacterDataTable& AGrux::GetCharacterStatData()
{
	return characterStat->GetCharacterStatData();
}