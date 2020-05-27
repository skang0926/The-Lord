// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoHandSword.h"
#include "BaseCharacter.h"
#include "Sound/SoundCue.h"
#include "CharacterDataTable.h"


// Sets default values
ATwoHandSword::ATwoHandSword()
{	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	twoHandSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TWOHANDSWORD"));
	attackRangeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	hitSwordSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HITSWORDSOUND"));
	guardSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GUARDSOUND"));
	parrySoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PARRYSOUND"));	
	skillModeSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SWAPSOUND"));
	skillAttackSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SKILLATTACKSOUND"));
	electronicParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ELECTRONICPARTICLE"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_EFFECT(TEXT("/Game/CloseCombat/Effects/Impacts/P_Imp_Metal.P_Imp_Metal"));

	for (int32 i = 0; i < 10; i++)
	{
		FName name = *FString::Printf(TEXT("EFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetupAttachment(twoHandSword);
		effect->SetRelativeLocation(FVector(0.0f, -130.0f, 0.0f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_EFFECT.Object);
	
		effectComponentArr.Add(effect);
	}

	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_ELECTRONICPARTICLE(TEXT("/Game/FXVarietyPack/Particles/P_ky_thunderStorm1.P_ky_thunderStorm1"));

	electronicParticleComponent->SetupAttachment(twoHandSword);
	electronicParticleComponent->SetRelativeLocationAndRotation(FVector(0.f, 100.f, 0.f), FRotator(0.f, 0.f, -90.f));
	electronicParticleComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	electronicParticleComponent->bAutoActivate = false;
	electronicParticleComponent->SetTemplate(P_ELECTRONICPARTICLE.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_ATTACKEFFECT(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2"));

	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("ATTACKEFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetRelativeLocation(FVector(0.0f, -130.0f, 0.0f));
		effect->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_ATTACKEFFECT.Object);

		attackEffectComponentArr.Add(effect);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_ELECTRONICATTACKEFFECT(TEXT("/Game/FXVarietyPack/Particles/P_ky_hit3.P_ky_hit3"));

	for (int32 i = 0; i < 3; i++)
	{
		FName name = *FString::Printf(TEXT("ELECTRONICATTACKEFFECT %d"), i);
		UParticleSystemComponent* effect = CreateDefaultSubobject<UParticleSystemComponent>(name);
		effect->SetRelativeLocation(FVector(0.0f, -130.0f, 0.0f));
		effect->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		effect->bAutoActivate = false;
		effect->SetTemplate(P_ELECTRONICATTACKEFFECT.Object);

		electronicAttackEffectComponentArr.Add(effect);
	}




	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_TWOHANDSWORD(TEXT("/Game/Items/TwoHandedSword/SM_TwoHandedSword.SM_TwoHandedSword"));
	if (SM_TWOHANDSWORD.Succeeded())
	{
		twoHandSword->SetStaticMesh(SM_TWOHANDSWORD.Object);
	}
	

	attackRangeCapsule->InitCapsuleSize(22.f, 44.f);
	attackRangeCapsule->SetRelativeLocationAndRotation(FVector(0.f, -90.f, 0.f), FRotator(0.f, 0.f, -90.f));
	attackRangeCapsule->SetRelativeScale3D(FVector(0.1f, 0.4f, 1.28f));
	attackRangeCapsule->SetGenerateOverlapEvents(true);

	RootComponent = twoHandSword;
	attackRangeCapsule->SetupAttachment(RootComponent);

	   	 		
	attackRangeCapsule->SetCollisionProfileName(TEXT("NoCollision"));
	twoHandSword->SetCollisionProfileName(TEXT("NoCollision"));	

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


void ATwoHandSword::BeginPlay()
{
	Super::BeginPlay();	
}

void ATwoHandSword::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	attackRangeCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATwoHandSword::OnAttackOverlap);

	
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
void ATwoHandSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	


}

void ATwoHandSword::OnAttackOverlap(UPrimitiveComponent* overlapComp, AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (otherActor == GetOwner()) return;
	
	ABaseCharacter* otherCharacter = dynamic_cast<ABaseCharacter*>(otherActor);
	if (!otherCharacter || otherCharacter->GetAnimState() == AnimState::DEAD) return;	

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
		ParticlePlay(electronicAttackEffectComponentArr, hitResult.Location);
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

void ATwoHandSword::OnEffect(FVector hitPos)
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

void ATwoHandSword::ParticlePlay(TArray<UParticleSystemComponent*> particleArr,FVector location)
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

void ATwoHandSword::PlayGuardSound()
{
	guardSoundComponent->Play();
}

void ATwoHandSword::PlayParrySound()
{
	parrySoundComponent->Play();
}

void ATwoHandSword::SetCollision(bool flag)
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

void ATwoHandSword::SetWeaponParticle(bool flag)
{
	bWeaponParticle = flag;
	
	if (flag)
	{
		electronicParticleComponent->Activate();
	}
	else
	{
		electronicParticleComponent->Deactivate();
	}

	skillModeSoundComponent->Play();

}