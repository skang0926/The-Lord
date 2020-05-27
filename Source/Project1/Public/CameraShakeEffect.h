// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "CameraShakeEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UCameraShakeEffect : public UCameraShake
{
	GENERATED_BODY()
public:
	UCameraShakeEffect();

	void SetCameraShakePower(float);

	void PlayShake(class APlayerCameraManager* Camera, float Scale, ECameraAnimPlaySpace::Type InPlaySpace, FRotator UserPlaySpaceRot = FRotator::ZeroRotator);
};
