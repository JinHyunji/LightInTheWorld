// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameData/LWCharacterStat.h"

#include "LWBaseCharacter.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class LIGHTINTHEWORLD_API ALWBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALWBaseCharacter();

	virtual void PostInitializeComponents() override;

// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 NewLevel);
	void ApplyStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWCharacterStatComponent> Stat;


// Control Data Section
protected:
	virtual void SetCharacterControlData(const class ULWCharacterControlData* CharacterControlData);
	
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class ULWCharacterControlData*> CharacterControlManager;


// Attack Hit Section
protected:
	//virtual void AttackHitCheck() override;


// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	virtual void PlayDeadAnimation();

	float DeadEventDelayTime = 5.f;
};
