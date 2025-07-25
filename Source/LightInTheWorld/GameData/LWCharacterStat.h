#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LWCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FLWCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLWCharacterStat() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	FLWCharacterStat operator+(const FLWCharacterStat& Other) const
	{
		FLWCharacterStat Result;
		Result.MaxHp = MaxHp + Other.MaxHp;
		Result.Attack = Attack + Other.Attack;
		Result.AttackRange = AttackRange + Other.AttackRange;
		Result.AttackSpeed = AttackSpeed + Other.AttackSpeed;
		Result.MovementSpeed = MovementSpeed + Other.MovementSpeed;
		return Result;
	}
};
