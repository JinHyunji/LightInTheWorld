// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/LWCharacterStatComponent.h"
#include "GameData/LWGameSingleton.h"

ULWCharacterStatComponent::ULWCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.f; // MBM

	bWantsInitializeComponent = true;

	UE_LOG(LogTemp, Warning, TEXT("ULWCharacterStatComponent::ULWCharacterStatComponent() - BaseStat.MaxHp: %f"), BaseStat.MaxHp);
}

void ULWCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);

	UE_LOG(LogTemp, Warning, TEXT("ULWCharacterStatComponent::InitializeComponent() - BaseStat.MaxHp: %f"), BaseStat.MaxHp);
}

float ULWCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// Dead
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void ULWCharacterStatComponent::SetLevelStat(int32 NewLevel)
{
	CurrentLevel = FMath::Clamp(NewLevel, 1, ULWGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(ULWGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.f);
}

void ULWCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}


