// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LWBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/LWCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LWCharacterControlData.h"
#include "Engine/DamageEvents.h"
#include "Physics/LWCollision.h"


ALWBaseCharacter::ALWBaseCharacter()
{
	// Pawn
	bUseControllerRotationRoll = false; // X
	bUseControllerRotationPitch = false; // Y
	bUseControllerRotationYaw = false; // Z

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 100.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_LWCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); // Pitch, Yaw, Roll
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Character Control Data
	static ConstructorHelpers::FObjectFinder<ULWCharacterControlData> ShoulderDataRef(TEXT("/Script/LightInTheWorld.LWCharacterControlData'/Game/LightInTheWorld/CharacterControl/LWC_Shoulder.LWC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Emplace(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<ULWCharacterControlData> QuaterDataRef(TEXT("/Script/LightInTheWorld.LWCharacterControlData'/Game/LightInTheWorld/CharacterControl/LWC_Quater.LWC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Emplace(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	// Stat Component
	Stat = CreateDefaultSubobject<ULWCharacterStatComponent>(TEXT("Stat"));
}


void ALWBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ALWBaseCharacter::SetDead);
	Stat->OnStatChanged.AddUObject(this, &ALWBaseCharacter::ApplyStat);
}

int32 ALWBaseCharacter::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void ALWBaseCharacter::SetLevel(int32 NewLevel)
{
	Stat->SetLevelStat(NewLevel);
}

void ALWBaseCharacter::ApplyStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void ALWBaseCharacter::SetCharacterControlData(const ULWCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

//void ALWBaseCharacter::AttackHitCheck()
//{
//	FHitResult OutHitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//	const float AttackRange = Stat->GetTotalStat().AttackRange;
//	const float AttackRadius = Stat->GetAttackRadius();
//	const float AttackDamage = Stat->GetTotalStat().Attack;
//	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
//	const FVector End = Start + GetActorForwardVector() * AttackRange;
//
//	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_LWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
//	if (HitDetected)
//	{
//		FDamageEvent DamageEvent;
//		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//	}
//
//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5;
//	float CapsuleHalfHeight = AttackRange * 0.5f + AttackRadius;
//	FColor DrawColor = HitDetected ? FColor:: Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
//	//DrawDebugCapsule(GetWorld(), GetActorLocation(), 88.f, 34.f, FQuat::Identity, FColor::Red, false, 5.f);
//#endif
//}

float ALWBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(Damage);

	return Damage;
}

void ALWBaseCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void ALWBaseCharacter::PlayDeadAnimation()
{

}