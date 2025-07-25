// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LWBaseCharacter.h"
#include "Interface/LWCharacterWidgetInterface.h"
#include "Interface/LWMonsterSkillInterface.h"

#include "LWMonsterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTINTHEWORLD_API ALWMonsterCharacter : public ALWBaseCharacter, public ILWCharacterWidgetInterface, public ILWMonsterSkillInterface
{
	GENERATED_BODY()
	
public:
	ALWMonsterCharacter();

protected:


// Attack Section
public:
	virtual void NormalAttack() override;


// Dead Section
protected:
	virtual void SetDead() override;
	virtual void PlayDeadAnimation() override;



// HpBar Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULWWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class ULWUserWidget* InUserWidget) override;




};
