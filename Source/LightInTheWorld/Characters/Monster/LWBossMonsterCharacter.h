// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Monster/LWMonsterCharacter.h"
#include "LWBossMonsterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ALWBossMonsterCharacter : public ALWMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ALWBossMonsterCharacter();

	virtual void NormalAttack() override;
	virtual void CastSkill(int32 SkillIndex) override;

	// Delegates

protected:
	void SkillOne();
	void SkillTwo();
	void SkillThree();
};
