// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LWWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ULWWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
