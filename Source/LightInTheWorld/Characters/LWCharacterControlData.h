// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LWCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ULWCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULWCharacterControlData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bDoCollisionTest : 1;
	
};
