// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LWHUDWidget.h"
#include "LWHpBarWidget.h"
#include "Interface/LWCharacterHUDInterface.h"

ULWHUDWidget::ULWHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULWHUDWidget::UpdateStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat)
{
	FLWCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->UpdateStat(BaseStat, ModifierStat);
}

void ULWHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void ULWHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<ULWHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	ILWCharacterHUDInterface* HUDPawn = Cast<ILWCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
