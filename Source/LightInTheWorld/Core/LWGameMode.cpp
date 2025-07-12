// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LWGameMode.h"
#include "Characters/Player/LWPlayerController.h"

ALWGameMode::ALWGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterPlayerClassRef = TEXT("/Script/Engine.Blueprint'/Game/LightInTheWorld/Blueprints/BP_LWPlayerCharacter.BP_LWPlayerCharacter_C'");
	if (CharacterPlayerClassRef.Class)
	{
		DefaultPawnClass = CharacterPlayerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef = TEXT("/Game/LightInTheWorld/Blueprints/BP_LWPlayerController.BP_LWPlayerController_C");
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
