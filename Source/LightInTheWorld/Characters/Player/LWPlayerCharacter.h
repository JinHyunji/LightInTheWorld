// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/LWAnimationAttackInterface.h"
#include "Interface/LWCharacterWidgetInterface.h"
#include "Interface/LWCharacterHUDInterface.h"
#include "GameData/LWCharacterStat.h"

#include "LWPlayerCharacter.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class LIGHTINTHEWORLD_API ALWPlayerCharacter : public ACharacter, public ILWAnimationAttackInterface, public ILWCharacterWidgetInterface, public ILWCharacterHUDInterface
{
	GENERATED_BODY()

public:
	ALWPlayerCharacter();

	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

// Control Data Section
protected:
	void SetCharacterControlData(const class ULWCharacterControlData* CharacterControlData);
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	void ChangeCharacterControl();

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class ULWCharacterControlData*> CharacterControlManager;

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
	TObjectPtr<class UInputAction> SkillAction;

// Movement Section
protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

// Attack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ExecuteComboAction();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0; // Attack Num
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	void Attack();

// Land Section
protected:
	virtual void Landed(const FHitResult& Hit) override;

private:
	bool bPendingAttackOnLanding = false;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 NewLevel);
	void ApplyStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWCharacterStatComponent> Stat;

// Dodge Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DodgeMontage;

	void DodgeActionBegin();
	void DodgeActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void PlayDodgeAnimation();

	void Dodge();

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

// UI Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class ULWUserWidget* InUserWidget) override;
	virtual void SetupHUDWidget(class ULWHUDWidget* InHUDWidget) override;

protected:
	void ShowInventory();

	void Interaction();

};
