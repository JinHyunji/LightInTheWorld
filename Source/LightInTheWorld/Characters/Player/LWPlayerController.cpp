// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LWPlayerController.h"
#include "UI/LWHUDWidget.h"

ALWPlayerController::ALWPlayerController()
{
	static ConstructorHelpers::FClassFinder<ULWHUDWidget> LWHUDWidgetRef(TEXT("/Game/LightInTheWorld/UI/WBP_LWHUD.WBP_LWHUD_C"));
	if (LWHUDWidgetRef.Class)
	{
		LWHUDWidgetClass = LWHUDWidgetRef.Class;
	}
}

void ALWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 마우스가 뷰포트 안으로 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);


}