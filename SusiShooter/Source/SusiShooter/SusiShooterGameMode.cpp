// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SusiShooter.h"
#include "SusiShooterGameMode.h"
#include "SusiShooterHUD.h"
#include "SusiShooterCharacter.h"


ASusiShooterGameMode::ASusiShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASusiShooterHUD::StaticClass();

	SusiLoader.Reset(new FSusiLoader());
	// TODO: READ FROM FILE
	//SusiLoader->Login(TEXT("USER"), TEXT("PASSWORD"));
}

void ASusiShooterGameMode::Tick(float Delta)
{
	/*SusiLoader->Tick(Delta);
	auto Info = SusiLoader->GetStudentInfo(TEXT("nikola1"));
	if (Info.Status == ESusiStatus::LoggedIn)
	{
		SusiLoader->RequestStudentInfo(TEXT("nikola1"));
	}*/

	// LOOK AT ME
	//auto PlayerController = GWorld->GetFirstPlayerController();
	//PlayerController->ClientMessage(FString("My Message"), NAME_None, 1.5f);

}


// LOOK AT ME
void ASusiShooterGameMode::ComputeTheAnswer()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("42!"));
}