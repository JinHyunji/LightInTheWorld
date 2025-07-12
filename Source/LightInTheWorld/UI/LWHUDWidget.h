// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/LWCharacterStat.h"

#include "LWHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ULWHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULWHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class ULWHpBarWidget> HpBar;

	// MBM : Add Any UIs

	
};
