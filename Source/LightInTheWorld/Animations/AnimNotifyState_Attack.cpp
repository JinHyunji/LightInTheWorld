// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_Attack.h"
#include "Interface/LWAnimationAttackInterface.h"

void UAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		ILWAnimationAttackInterface* AttackPawn = Cast<ILWAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->EnableSwordTrace();
		}
	}
}

void UAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ILWAnimationAttackInterface* AttackPawn = Cast<ILWAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->DisableSwordTrace();
		}
	}
}
