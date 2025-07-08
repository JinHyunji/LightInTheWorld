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

ALWPlayerCharacter::ALWPlayerCharacter()
{
	// Pawn
	bUseControllerRotationRoll = false; // X
	bUseControllerRotationPitch = false; // Y
	bUseControllerRotationYaw = false; // Z

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 100.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn")); // Must be modified

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
}

void ALWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Dodge);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::Attack);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ALWPlayerCharacter::QuaterMove);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::ShowInventory);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ALWPlayerCharacter::Interaction);

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
	UE_LOG(LogTemp, Log, TEXT("Pressed Attack"));

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
	UE_LOG(LogTemp, Log, TEXT("Pressed Dodge"));
}


