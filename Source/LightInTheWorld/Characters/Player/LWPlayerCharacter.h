// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/LWBaseCharacter.h"
#include "Interface/LWCharacterHUDInterface.h"
#include "Interface/LWAnimationAttackInterface.h"

#include "LWPlayerCharacter.generated.h"

UCLASS()
class LIGHTINTHEWORLD_API ALWPlayerCharacter : public ALWBaseCharacter, public ILWCharacterHUDInterface, public ILWAnimationAttackInterface
{
	GENERATED_BODY()

public:
	ALWPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class ULWCharacterControlData* CharacterControlData) override;

	ECharacterControlType CurrentCharacterControlType;


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> UltimateAction;


// Movement Section
protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);


// Attack Section
protected:

	void Attack();
	void TraceSword();
	virtual void EnableSwordTrace() override;
	virtual void DisableSwordTrace() override;
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TSubclassOf<UCameraShakeBase> HitCameraShake;


	bool bIsAttacking = false;

	virtual void AttackHitCheck() override;


	// Not used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void ExecuteComboAction();
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0; // Attack Num
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

private:
	UPROPERTY()
	TSet<TObjectPtr<ALWBaseCharacter>> HitActors; // 중복 타격 방지
	FTimerHandle SwordTraceTimerHandle;


// Skill & Ultimate Section
protected:
	void SkillOne();
	void SkillTwo();
	void Ultimate();


// Dead Section
protected:
	virtual void PlayDeadAnimation() override;

// Land Section
protected:
	virtual void Landed(const FHitResult& Hit) override;

private:
	bool bPendingAttackOnLanding = false;


// Dodge Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DodgeMontage;

	void DodgeActionBegin();
	void DodgeActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void PlayDodgeAnimation();

	void Dodge();


// UI Section
protected:
	virtual void SetupHUDWidget(class ULWHUDWidget* InHUDWidget) override;

protected:
	void ShowInventory();

	void Interaction();

};
