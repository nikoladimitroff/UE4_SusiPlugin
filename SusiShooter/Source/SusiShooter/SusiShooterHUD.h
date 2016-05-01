// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "SSusiLoginScreen.h"
#include "SusiShooterHUD.generated.h"

UCLASS()
class ASusiShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASusiShooterHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	TSharedPtr<SSusiLoginScreen> LoginScreen;

};

