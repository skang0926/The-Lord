// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "Weapon.h"
#include "OneHandSword.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API AOneHandSword : public AWeapon
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AOneHandSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = TWOHANDSWORD)
		UStaticMeshComponent* oneHandSword;

	UPROPERTY(VisibleAnywhere, Category = AttackRange)
		UCapsuleComponent* attackRangeCapsule;

private:
	UFUNCTION()
		void OnAttackOverlap(UPrimitiveComponent* overlapComp, AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* hitSwordSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* hitSwordSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* guardSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* guardSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* parrySoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* parrySoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* skillModeSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* skillModeSoundComponent;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		class USoundCue* skillAttackSoundCue;

	UPROPERTY(VisibleAnywhere, Category = SOUND)
		UAudioComponent* skillAttackSoundComponent;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> effectComponentArr;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> attackEffectComponentArr;

	UPROPERTY(VisibleAnywhere)
		TArray<UParticleSystemComponent*> skillAttackEffectComponentArr;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* weapomParticleComponent;


	bool bWeaponParticle = false;
public:
	void OnEffect(FVector);
	void ParticlePlay(TArray<UParticleSystemComponent*>, FVector);
	void PlayGuardSound();
	void PlayParrySound();

	void SetCollision(bool);

	void SetWeaponParticle(bool);
};
