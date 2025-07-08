// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LWPlayerController.h"

ALWPlayerController::ALWPlayerController()
{

}

void ALWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 마우스가 뷰포트 안으로 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}