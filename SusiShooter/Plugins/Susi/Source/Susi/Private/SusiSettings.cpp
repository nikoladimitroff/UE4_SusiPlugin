#include "SusiPrivatePCH.h"
#include "SusiSettings.h"

USusiSettings::USusiSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TextColor(FLinearColor::Red)
	, BorderColor(0.18f, 0.31f, 0.31f)
	, BackgroundColor(0.18f, 0.18f, 0.18f)
	, Font(TEXT("Verdana"), 30u)
	, PasswordFile("PasswordFile.txt")
{
}