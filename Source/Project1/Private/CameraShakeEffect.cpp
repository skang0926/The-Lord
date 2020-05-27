// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeEffect.h"

UCameraShakeEffect::UCameraShakeEffect()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	float rangeMin = 0.4f;
	float rangeMax = 0.6f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(rangeMin, rangeMax);
	RotOscillation.Pitch.Frequency = FMath::RandRange(rangeMin, rangeMax);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(rangeMin, rangeMax);
	RotOscillation.Yaw.Frequency = FMath::RandRange(rangeMin, rangeMax);
}

void UCameraShakeEffect::PlayShake(APlayerCameraManager* Camera, float Scale, ECameraAnimPlaySpace::Type InPlaySpace, FRotator UserPlaySpaceRot)
{	
	Super::PlayShake(Camera, Scale, InPlaySpace, UserPlaySpaceRot);
}