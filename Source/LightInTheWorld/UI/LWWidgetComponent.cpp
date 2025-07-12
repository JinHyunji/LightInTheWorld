// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LWWidgetComponent.h"
#include "LWUserWidget.h"

void ULWWidgetComponent::InitWidget()
{
	Super::InitWidget();

	ULWUserWidget* LWUserWidget = Cast<ULWUserWidget>(GetWidget());
	if (LWUserWidget)
	{
		LWUserWidget->SetOwningActor(GetOwner());
	}
}