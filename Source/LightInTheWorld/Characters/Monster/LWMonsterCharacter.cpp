// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/LWMonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UI/LWHpBarWidget.h"
#include "Characters/Components/LWCharacterStatComponent.h"
#include "LWBossMonsterCharacter.h"
#include "UI/LWWidgetComponent.h"



ALWMonsterCharacter::ALWMonsterCharacter()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Skins/Tier1/WhiteTiger/Mesh/Khaimera_WhiteTiger.Khaimera_WhiteTiger'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    // Animation 
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/LightInTheWorld/Animation/Monster/ABP_MonsterCharacter.ABP_MonsterCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    // Dead Montage
    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/Monster/AM_Monster_Dead.AM_Monster_Dead'"));
    if (DeadMontageRef.Object)
    {
        DeadMontage = DeadMontageRef.Object;
    }

    HpBar = CreateDefaultSubobject<ULWWidgetComponent>(TEXT("Widget"));
    HpBar->SetupAttachment(GetMesh());
    HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/LightInTheWorld/UI/WBP_HpBar.WBP_HpBar_C"));
    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);
        HpBar->SetDrawSize(FVector2D(170.f, 20.f));
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void ALWMonsterCharacter::NormalAttack()
{

}

void ALWMonsterCharacter::SetDead()
{
    Super::SetDead();

    HpBar->SetHiddenInGame(true);

    FTimerHandle DeadTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
        [&]()
        {
            Destroy();
        }
    ), DeadEventDelayTime, false);
}

void ALWMonsterCharacter::PlayDeadAnimation()
{
    Super::PlayDeadAnimation();

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->StopAllMontages(0.f);
        AnimInstance->Montage_Play(DeadMontage);
    }
}

void ALWMonsterCharacter::SetupCharacterWidget(ULWUserWidget* InUserWidget)
{
    ULWHpBarWidget* HpBarWidget = Cast<ULWHpBarWidget>(InUserWidget);
    if (HpBarWidget)
    {
        UE_LOG(LogTemp, Log, TEXT("MaxHp : %f"), Stat->GetBaseStat().MaxHp);
        HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
        Stat->OnHpChanged.AddUObject(HpBarWidget, &ULWHpBarWidget::UpdateHpBar);
        Stat->OnStatChanged.AddUObject(HpBarWidget, &ULWHpBarWidget::UpdateStat);
    }
}

