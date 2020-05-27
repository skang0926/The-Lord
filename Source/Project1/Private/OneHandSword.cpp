// Fill out your copyright notice in the Description page of Project Settings.


#include "OneHandSword.h"
#include "BaseCharacter.h"
#include "Sound/SoundCue.h"
#include "CharacterDataTable.h"

AOneHandSword::AOneHandSword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	oneHandSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ONEHANDSWORD"));
	attackRangeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	hitSwordSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HITSWORDSOUND"));
	guardSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GUARDSOUND"));
	parrySoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PARRYSOUND"));
	skillModeSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SWAPSOUND"));
	skillAttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SKILLATTACKSOUND"));
	weapomParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ELECTRONICPARTICLE"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_EFFECT(TEXT("/Game/CloseCombat/Effects/Impacts/P_Imp_Metal.P_Imp_Metal"));

	for (int32 i = 0; i < 10; i++)
	{
		FName name = *FString::Printf(TEXT("EFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetupAttachment(oneHandSword);
		effect->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_EFFECT.Object);

		effectComponentArr.Add(effect);
	}
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_WEAPONPARTICLE(TEXT("/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm"));

	weapomParticleComponent->SetupAttachment(oneHandSword);
	weapomParticleComponent->SetRelativeLocationAndRotation(FVector(0.f, 100.f, 0.f), FRotator(0.f, 0.f, -90.f));
	weapomParticleComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	weapomParticleComponent->bAutoActivate = false;
	weapomParticleComponent->SetTemplate(P_WEAPONPARTICLE.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_ATTACKEFFECT(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2"));

	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("ATTACKEFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		effect->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_ATTACKEFFECT.Object);

		attackEffectComponentArr.Add(effect);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_SKILLATTACKEFFECT(TEXT("/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion"));

	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("ELECTRONICATTACKEFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		effect->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_SKILLATTACKEFFECT.Object);

		skillAttackEffectComponentArr.Add(effect);
	}




	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_ONEHANDSWORD(TEXT("/Game/Characters/GhostLady_S2/Animations/WARRIOR_SWORD_ANIMSET/Sword_Warrior/Mesh/SM_Sword.SM_Sword"));
	if (SM_ONEHANDSWORD.Succeeded())
	{
		oneHandSword->SetStaticMesh(SM_ONEHANDSWORD.Object);
	}
	
	attackRangeCapsule->InitCapsuleSize(22.f, 44.f);
	attackRangeCapsule->SetRelativeLocationAndRotation(FVector(0.f, -40.0, 0.f), FRotator(0.f, 0.f, -90.f));
	attackRangeCapsule->SetRelativeScale3D(FVector(1.0f, 0.5f, 1.f));
	attackRangeCapsule->SetGenerateOverlapEvents(true);

	RootComponent = oneHandSword;
	attackRangeCapsule->SetupAttachment(RootComponent);


	attackRangeCapsule->SetCollisionProfileName(TEXT("NoCollision"));
	oneHandSword->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USoundCue> HITSWORDSOUNDCUE(TEXT("/Game/Sound/A_RandHitSword.A_RandHitSword"));
	if (HITSWORDSOUNDCUE.Succeeded())
	{
		hitSwordSoundCue = HITSWORDSOUNDCUE.Object;
	}

	hitSwordSoundComponent->bAutoActivate = false;
	hitSwordSoundComponent->SetupAttachment(RootComponent);
	hitSwordSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> GUARDSOUNDCUE(TEXT("/Game/Sound/A_GuardCue.A_GuardCue"));
	if (GUARDSOUNDCUE.Succeeded())
	{
		guardSoundCue = GUARDSOUNDCUE.Object;
	}

	guardSoundComponent->bAutoActivate = false;
	guardSoundComponent->SetupAttachment(RootComponent);
	guardSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> PARRYSOUNDCUE(TEXT("/Game/Sound/A_ParryCue.A_ParryCue"));
	if (PARRYSOUNDCUE.Succeeded())
	{
		parrySoundCue = PARRYSOUNDCUE.Object;
	}

	parrySoundComponent->bAutoActivate = false;
	parrySoundComponent->SetupAttachment(RootComponent);
	parrySoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundCue> SWAPSOUNDCUE(TEXT("/Game/DarkMagic/DarkMagicCue/OnSkillMode.OnSkillMode"));
	if (SWAPSOUNDCUE.Succeeded())
	{
		skillModeSoundCue = SWAPSOUNDCUE.Object;
	}

	skillModeSoundComponent->bAutoActivate = false;
	skillModeSoundComponent->SetupAttachment(RootComponent);
	skillModeSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USoundCue> SKILLATTACKSOUNDCUE(TEXT("/Game/DarkMagic/DarkMagicCue/ElectronicAttackSoundCue.ElectronicAttackSoundCue"));
	if (SKILLATTACKSOUNDCUE.Succeeded())
	{
		skillAttackSoundCue = SKILLATTACKSOUNDCUE.Object;
	}

	skillAttackSoundComponent->bAutoActivate = false;
	skillAttackSoundComponent->SetupAttachment(RootComponent);
	skillAttackSoundComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}


// Called when the game starts or when spawned


void AOneHandSword::BeginPlay()
{
	Super::BeginPlay();
}

void AOneHandSword::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	attackRangeCapsule->OnComponentBeginOverlap.AddDynamic(this, &AOneHandSword::OnAttackOverlap);


	if (hitSwordSoundCue->IsValidLowLevelFast())
	{
		hitSwordSoundComponent->SetSound(hitSwordSoundCue);
	}

	if (guardSoundCue->IsValidLowLevelFast())
	{
		guardSoundComponent->SetSound(guardSoundCue);
	}

	if (parrySoundCue->IsValidLowLevelFast())
	{
		parrySoundComponent->SetSound(parrySoundCue);
	}

	if (skillModeSoundCue->IsValidLowLevelFast())
	{
		skillModeSoundComponent->SetSound(skillModeSoundCue);
	}

	if (skillAttackSoundCue->IsValidLowLevelFast())
	{
		skillAttackSoundComponent->SetSound(skillAttackSoundCue);
	}
}


// Called every frame
void AOneHandSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AOneHandSword::OnAttackOverlap(UPrimitiveComponent* overlapComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (otherActor == GetOwner()) return;

	ABaseCharacter* otherCharacter = dynamic_cast<ABaseCharacter*>(otherActor);
	if (!otherCharacter) return;

	FHitResult hitResult = SweepResult;
	if (hitResult.Location == FVector::ZeroVector)
	{
		hitResult.Location = effectComponentArr.Top()->GetComponentLocation();
	}

	ABaseCharacter* owner = dynamic_cast<ABaseCharacter*>(GetOwner());

	if (owner->GetCharacterStatData().faction == otherCharacter->GetCharacterStatData().faction)
		return;

	float damage = owner->GetCharacterStatData().attackPower;
	TSubclassOf<class UDamageType> types;


	if (bWeaponParticle)
	{
		ParticlePlay(skillAttackEffectComponentArr, hitResult.Location);
		owner->ShakeEffect(5.f);
		skillAttackSoundComponent->Play();
		damage *= 2;
	}
	else
	{
		ParticlePlay(attackEffectComponentArr, hitResult.Location);
		owner->ShakeEffect(1.f);
	}


	FPointDamageEvent damageEvent(damage, hitResult, FVector::ZeroVector, types);


	float availableDamage = otherCharacter->TakeDamage(damage, damageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	if (availableDamage == damage)
		hitSwordSoundComponent->Play();

	SetCollision(false);
}

void AOneHandSword::OnEffect(FVector hitPos)
{
	for (auto effect : effectComponentArr)
	{
		if (!effect->IsActive())
		{
			effect->SetRelativeLocationAndRotation(hitPos - GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
			effect->Activate();
			return;
		}
	}
}

void AOneHandSword::ParticlePlay(TArray<UParticleSystemComponent*> particleArr, FVector location)
{
	for (auto effect : particleArr)
	{
		if (!effect->IsActive())
		{
			effect->SetRelativeLocationAndRotation(location, FRotator::ZeroRotator);
			effect->Activate();
			break;
		}
	}
}

void AOneHandSword::PlayGuardSound()
{
	guardSoundComponent->Play();
}

void AOneHandSword::PlayParrySound()
{
	parrySoundComponent->Play();
}

void AOneHandSword::SetCollision(bool flag)
{
	if (flag)
	{
		attackRangeCapsule->SetCollisionProfileName(TEXT("OverlapAll"));
	}
	else
	{
		attackRangeCapsule->SetCollisionProfileName(TEXT("NoCollision"));
	}

}

void AOneHandSword::SetWeaponParticle(bool flag)
{
	bWeaponParticle = flag;

	if (flag)
	{
		weapomParticleComponent->Activate();
	}
	else
	{
		weapomParticleComponent->Deactivate();
	}

	skillModeSoundComponent->Play();

}