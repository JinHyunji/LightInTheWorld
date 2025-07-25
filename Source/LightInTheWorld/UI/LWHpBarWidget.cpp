// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LWHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/LWCharacterWidgetInterface.h"
#include "LWHUDWidget.h"


ULWHpBarWidget::ULWHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.f;

	UE_LOG(LogTemp, Warning, TEXT("ULWHpBarWidget::ULWHpBarWidget() - BaseStat.MaxHp: %f"), MaxHp);
}

void ULWHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	UE_LOG(LogTemp, Warning, TEXT("ULWHpBarWidget::NativeConstruct() - BaseStat.MaxHp: %f"), MaxHp);

	ILWCharacterWidgetInterface* CharacterWidget = Cast<ILWCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void ULWHpBarWidget::UpdateStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void ULWHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	UE_LOG(LogTemp, Warning, TEXT("ULWHpBarWidget::UpdateHpBar() - BaseStat.MaxHp: %f"), MaxHp);


	if (MaxHp <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("ULWHpBarWidget::UpdateHpBar - MaxHp not yet initialized! Skipping."));
		return;
	}

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
	
	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString ULWHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp);
}

