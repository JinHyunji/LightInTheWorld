// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/LWCharacterStat.h"
#include "LWCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* Current Hp */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FLWCharacterStat&, const FLWCharacterStat&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTINTHEWORLD_API ULWCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULWCharacterStatComponent();

	float ApplyDamage(float InDamage);

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }

	void SetLevelStat(int32 NewLevel);
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	FORCEINLINE const FLWCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE void SetBaseStat(const FLWCharacterStat& NewBaseStat) { BaseStat = NewBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void AddBaseStat(const FLWCharacterStat& AddBaseStat) { BaseStat = BaseStat + AddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE const FLWCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE void SetModifierStat(const FLWCharacterStat& NewModifierStat) { ModifierStat = NewModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE FLWCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

protected:
	virtual void InitializeComponent() override;
	void SetHp(float NewHp);

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FLWCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FLWCharacterStat ModifierStat;


		
};
