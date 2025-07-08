// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ALWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALWPlayerController();

protected:
	virtual void BeginPlay() override;
};
