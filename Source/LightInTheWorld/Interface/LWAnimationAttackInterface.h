// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LWAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULWAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTINTHEWORLD_API ILWAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackHitCheck() = 0;
	virtual void EnableSwordTrace() = 0;
	virtual void DisableSwordTrace() = 0;
};
