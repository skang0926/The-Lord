// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project1.h"
#include "GameFramework/PlayerController.h"
#include "TutorialPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDynamicTutorialChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTutorialChangedDelegate);
/**
 * 
 */
UENUM()
enum TutorialState
{
	MoveTutorial UMETA(DisplayName = "Move"),
	EquipTutorial UMETA(DisplayName = "Equip"),	
	LockOnTutorial UMETA(DisplayName = "LockOn"),
	InPlaceAttackTutorial UMETA(DisplayName = "InPlaceAttackTutorial"),
	ForwardAttackTutorial UMETA(DisplayName = "ForwardAttackTutorial"),
	PierceAttackTutorial UMETA(DisplayName = "PierceAttackTutorial"),
	SkillATutorial UMETA(DisplayName = "SkillA"),
	SkillBTutorial  UMETA(DisplayName = "SkillB"),	
	SkillModeTutorial UMETA(DisplayName = "SkillMode"),
	SkillAttackTutorial UMETA(DisplayName = "SkillModeAttack"),
	MoveToGuardTutorial UMETA(DisplayName = "MoveToGuardTutorial"),
	GuardTutorial UMETA(DisplayName = "GuardTutorial"),
	ParryTutorial UMETA(DisplayName = "ParryTutorial"),
	ParryCountTutorial UMETA(DisplayName = "ParryCountTutorial"),
	CompleteTutorial UMETA(DisplayName = "CompleteTutorial")
};

UCLASS(BlueprintType, Blueprintable)
class PROJECT1_API ATutorialPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	FInputModeGameOnly gameInputMode;
	FInputModeUIOnly UIInputMode;
	
	ATutorialPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UUserHUDWidget* HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UTutorialWidget> tutorialWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UTutorialWidget* tutorialWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GamePlayState, Meta = (AllowPrivateAccess = true))
		TEnumAsByte<TutorialState> tutorialState = TutorialState::MoveTutorial;

	UPROPERTY(BlueprintAssignable)
	FOnDynamicTutorialChangedDelegate OnTutorialChange;
	FOnTutorialChangedDelegate OnTutorialStateChange;
public:
	UFUNCTION(BlueprintCallable, Category = State)
	void SetChangeTutorialState(TutorialState state);

	UFUNCTION(BlueprintCallable, Category = State)
	TutorialState GetTutorialState();

	void ChangeInputMode(bool bInputMode = true);
};