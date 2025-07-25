// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LWCharacterStat.h"

#include "LWGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ULWGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	ULWGameSingleton();
	static ULWGameSingleton& Get();

public:
	FORCEINLINE FLWCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FLWCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FLWCharacterStat> CharacterStatTable;
};
