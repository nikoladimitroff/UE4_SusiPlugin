// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "SusiLoader.h"
#include "SusiShooterGameMode.generated.h"

UCLASS(minimalapi)
class ASusiShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASusiShooterGameMode();
	void Tick(float Delta) override;

	UFUNCTION(exec)
	void ComputeTheAnswer();
private:
	TUniquePtr<FSusiLoader> SusiLoader;
};



