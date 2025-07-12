// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_AttackHitCheck.h"
#include "Interface/LWAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ILWAnimationAttackInterface* AttackPawn = Cast<ILWAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}