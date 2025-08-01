// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LWCharacterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULWCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTINTHEWORLD_API ILWCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void SetupCharacterWidget(class ULWUserWidget* InUserWidget) = 0;
};
