// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LWPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/LWCharacterControlData.h"
#include "LWComboActionData.h"
#include "Characters/Components/LWCharacterStatComponent.h"
#include "Physics/LWCollision.h"
#include "UI/LWWidgetComponent.h"
#include "UI/LWHpBarWidget.h"
#include "UI/LWHUDWidget.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"


ALWPlayerCharacter::ALWPlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/FrozenHearth/Meshes/Aurora_FrozenHearth.Aurora_FrozenHearth'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/LightInTheWorld/Animation/ABP_PlayerCharacter.ABP_PlayerCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;


	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputJumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputJumpActionRef.Object)
	{
		JumpAction = InputJumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeControlActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (InputChangeControlActionRef.Object)
	{
		ChangeControlAction = InputChangeControlActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputShoulderMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (InputShoulderMoveActionRef.Object)
	{
		ShoulderMoveAction = InputShoulderMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputShoulderLookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (InputShoulderLookActionRef.Object)
	{
		ShoulderLookAction = InputShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputQuaterMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (InputQuaterMoveActionRef.Object)
	{
		QuaterMoveAction = InputQuaterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputDodgeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Dodge.IA_Dodge'"));
	if (InputDodgeActionRef.Object)
	{
		DodgeAction = InputDodgeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputInventoryActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Inventory.IA_Inventory'"));
	if (InputInventoryActionRef.Object)
	{
		InventoryAction = InputInventoryActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputInteractionActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Interaction.IA_Interaction'"));
	if (InputInteractionActionRef.Object)
	{
		InteractionAction = InputInteractionActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkillOneActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Skill1.IA_Skill1'"));
	if (InputSkillOneActionRef.Object)
	{
		SkillOneAction = InputSkillOneActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkillTwoActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Skill2.IA_Skill2'"));
	if (InputSkillTwoActionRef.Object)
	{
		SkillTwoAction = InputSkillTwoActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputUltimateActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Ultimate.IA_Ultimate'"));
	if (InputUltimateActionRef.Object)
	{
		UltimateAction = InputUltimateActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputAttackActionRef.Object)
	{
		AttackAction = InputAttackActionRef.Object;
	}

	// Combo Action
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/AM_Attack.AM_Attack'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<ULWComboActionData> ComboActionDataRef(TEXT("/Script/LightInTheWorld.LWComboActionData'/Game/LightInTheWorld/CharacterAction/LWA_ComboAttack.LWA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// Dead Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/AM_Dodge.AM_Dodge'"));
	if (DodgeMontageRef.Object)
	{
		DodgeMontage = DodgeMontageRef.Object;
	}
}

void ALWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Dodge);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Attack);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::QuaterMove);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::ShowInventory);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Interaction);
	EnhancedInputComponent->BindAction(SkillOneAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::SkillOne);
	EnhancedInputComponent->BindAction(SkillTwoAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::SkillTwo);
	EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Ultimate);

}

void ALWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void ALWPlayerCharacter::SetCharacterControlData(const class ULWCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);
	
	// Camera
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ALWPlayerCharacter::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	ULWCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ALWPlayerCharacter::ShoulderMove(const FInputActionValue& Value)
{
	if (bIsAttacking) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ALWPlayerCharacter::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ALWPlayerCharacter::QuaterMove(const FInputActionValue& Value)
{
	if (bIsAttacking) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ALWPlayerCharacter::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ALWPlayerCharacter::ComboActionBegin()
{
	if (GetCharacterMovement()->IsFalling())
	{
		bPendingAttackOnLanding = true;
		return;
	}

	ExecuteComboAction();
}

void ALWPlayerCharacter::ExecuteComboAction()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ALWPlayerCharacter::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ALWPlayerCharacter::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void ALWPlayerCharacter::NotifyComboActionEnd()
{
}

void ALWPlayerCharacter::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ALWPlayerCharacter::ComboCheck, ComboEffectiveTime, false);
	}
}

void ALWPlayerCharacter::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void ALWPlayerCharacter::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ALWPlayerCharacter::Attack()
{
	//ProcessComboCommand();

	if (bIsAttacking) return;

	bIsAttacking = true;

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage); // AttackSpeedRate ? MBM

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ALWPlayerCharacter::OnAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}
}

void ALWPlayerCharacter::TraceSword()
{
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FVector Start = GetMesh()->GetSocketLocation(TEXT("Sword_Base"));
	FVector End = GetMesh()->GetSocketLocation(TEXT("Sword_Tip"));
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Attack;

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_LWACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bHit)
	{
		ALWBaseCharacter* HitActor = Cast<ALWBaseCharacter>(Hit.GetActor());

		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);

			// 피격 처리
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

			// 이펙트 -> MBM
			if (HitEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					HitEffect,
					Hit.ImpactPoint,
					Hit.ImpactNormal.Rotation()
				);
			}

			// MBM
			//if (HitCameraShake) 
			//{
			//	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
			//}
		}
	}

	FVector CapsuleOrigin = (Start + End) * 0.5f;
	float CapsuleHalfHeight = (Start - End).Size() * 0.5f;
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(
		GetWorld(), 
		CapsuleOrigin, 
		CapsuleHalfHeight, 
		AttackRadius, 
		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		DrawColor, 
		false, 
		2.f
	);
}

void ALWPlayerCharacter::EnableSwordTrace()
{
	HitActors.Empty();

	GetWorld()->GetTimerManager().SetTimer(
		SwordTraceTimerHandle,
		this,
		&ALWPlayerCharacter::TraceSword,
		0.02f,
		true
	);
}

void ALWPlayerCharacter::DisableSwordTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(SwordTraceTimerHandle);
	HitActors.Empty();
}

void ALWPlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
}

void ALWPlayerCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_LWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5;
	float CapsuleHalfHeight = AttackRange * 0.5f + AttackRadius;
	FColor DrawColor = HitDetected ? FColor:: Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

void ALWPlayerCharacter::SkillOne()
{
	UE_LOG(LogTemp, Log, TEXT("Pressed SkillOne"));

}

void ALWPlayerCharacter::SkillTwo()
{
	UE_LOG(LogTemp, Log, TEXT("Pressed SkillTwo"));

}

void ALWPlayerCharacter::Ultimate()
{
	UE_LOG(LogTemp, Log, TEXT("Pressed Ultimate"));

}

void ALWPlayerCharacter::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(DeadMontage, 1.f);
	}
}

void ALWPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bPendingAttackOnLanding)
	{
		bPendingAttackOnLanding = false;
		ComboActionBegin();
	}
}

void ALWPlayerCharacter::SetupHUDWidget(ULWHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());

		Stat->OnHpChanged.AddUObject(InHUDWidget, &ULWHUDWidget::UpdateHpBar);
		Stat->OnStatChanged.AddUObject(InHUDWidget, &ULWHUDWidget::UpdateStat);
	}
}

void ALWPlayerCharacter::ShowInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Pressed ShowInventory"));
}

void ALWPlayerCharacter::Interaction()
{
	UE_LOG(LogTemp, Log, TEXT("Pressed Interaction"));
}

void ALWPlayerCharacter::Dodge()
{
	DodgeActionBegin();
}

void ALWPlayerCharacter::DodgeActionBegin()
{
	/*if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}*/
	UCharacterMovementComponent* Movement = GetCharacterMovement();

	// 공중에 있거나, 정지됐거나, 몽타주가 아직 실행 중인 경우는 return
	if (Movement->IsFalling()) return; 
	if (Movement->GetLastInputVector() == FVector(0, 0, 0)) return;
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(DodgeMontage)) return;
	PlayDodgeAnimation();
}

void ALWPlayerCharacter::DodgeActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	/*if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
	}*/
}

void ALWPlayerCharacter::PlayDodgeAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DodgeMontage, 1.5f);
	} 

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ALWPlayerCharacter::DodgeActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
}


