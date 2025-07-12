// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LWUserWidget.h"
#include "GameData/LWCharacterStat.h"
#include "LWHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ULWHpBarWidget : public ULWUserWidget
{
	GENERATED_BODY()
	
public:
	ULWHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

};
