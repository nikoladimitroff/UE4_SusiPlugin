#pragma once

#include "UObject.h"
#include "Fonts/SlateFontInfo.h"
#include "SusiSettings.generated.h"

UCLASS(Config=Game, DefaultConfig)
class SUSI_API USusiSettings : public UObject
{
	GENERATED_BODY()

public:
	USusiSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/// The color used for drawing text by the plugin
	/// (e.g. in SSusiLoginScreen)
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FLinearColor TextColor;

	/// The color used for drawing UI background by the plugin
	/// (e.g. in SSusiLoginScreen)
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FLinearColor BackgroundColor;

	/// The color used for drawing the border around UI by the plugin
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FLinearColor BorderColor;

	/// The font used for drawing the border around UI by the plugin
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FSlateFontInfo Font;

	/// Path to the file containing the password for the users
	/// that will be logged using USusiComponent
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	FString PasswordFile;
};
