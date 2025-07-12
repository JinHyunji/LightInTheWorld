// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LWPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LWCharacterControlData.h"
#include "LWComboActionData.h"
#include "Characters/Components/LWCharacterStatComponent.h"
#include "Physics/LWCollision.h"
#include "UI/LWWidgetComponent.h"
#include "UI/LWHpBarWidget.h"
#include "UI/LWHUDWidget.h"
#include "Engine/DamageEvents.h"

ALWPlayerCharacter::ALWPlayerCharacter()
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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkillActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Skill.IA_Skill'"));
	if (InputSkillActionRef.Object)
	{
		SkillAction = InputSkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/LightInTheWorld/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputAttackActionRef.Object)
	{
		AttackAction = InputAttackActionRef.Object;
	}

	// Combo Action
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

	// Dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/LightInTheWorld/Animation/AM_Dodge.AM_Dodge'"));
	if (DodgeMontageRef.Object)
	{
		DodgeMontage = DodgeMontageRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<ULWCharacterStatComponent>(TEXT("Stat"));

	// Character Widget Component
	//HpBar = CreateDefaultSubobject<ULWWidgetComponent>(TEXT("Widget"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/LightInTheWorld/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen); // 2D
	//	HpBar->SetDrawSize(FVector2D(190.f, 25.f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
}

void ALWPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Stat->OnHpZero.AddUObject(this, &ALWPlayerCharacter::SetDead);
	Stat->OnStatChanged.AddUObject(this, &ALWPlayerCharacter::ApplyStat);
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

}

int32 ALWPlayerCharacter::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void ALWPlayerCharacter::SetLevel(int32 NewLevel)
{
	Stat->SetLevelStat(NewLevel);
}

void ALWPlayerCharacter::ApplyStat(const FLWCharacterStat& BaseStat, const FLWCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
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

void ALWPlayerCharacter::SetCharacterControlData(const ULWCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

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
	ProcessComboCommand();
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
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
	//DrawDebugCapsule(GetWorld(), GetActorLocation(), 88.f, 34.f, FQuat::Identity, FColor::Red, false, 5.f);
#endif
}

float ALWPlayerCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(Damage);

	return Damage;
}


void ALWPlayerCharacter::SetupCharacterWidget(ULWUserWidget* InUserWidget)
{
	ULWHpBarWidget* HpBarWidget = Cast<ULWHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &ULWHpBarWidget::UpdateHpBar);
		Stat->OnStatChanged.AddUObject(HpBarWidget, &ULWHpBarWidget::UpdateStat);
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

void ALWPlayerCharacter::SetDead()
{
}

void ALWPlayerCharacter::PlayDeadAnimation()
{
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


