// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/LWGameSingleton.h"

ULWGameSingleton::ULWGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/LightInTheWorld/GameData/LWCharacterStatTable.LWCharacterStatTable'"));
	if (DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FLWCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

ULWGameSingleton& ULWGameSingleton::Get()
{
	ULWGameSingleton* Singleton = CastChecked<ULWGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Error, TEXT("InValid Game Singleton"));
	return *NewObject<ULWGameSingleton>();
}