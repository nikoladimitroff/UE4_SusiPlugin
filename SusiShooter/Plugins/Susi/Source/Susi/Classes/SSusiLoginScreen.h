#pragma once

#include "SlateBasics.h"
#include "SCompoundWidget.h"
#include "SusiLoader.h"
#include "SusiSettings.h"

class SUSI_API SSusiLoginScreen : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSusiLoginScreen)
	{}
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& Args);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	struct FLoginScreenData
	{
		TSharedPtr<SEditableText> UsernameField;
		TSharedPtr<SEditableText> PasswordField;
		TSharedPtr<STextBlock> ErrorField;
	} LoginScreenData;
	struct FScreenStyle
	{
		const FLinearColor FillColor = FLinearColor(0.4f, 0.4f, 0.4f);
		const FLinearColor ShadowColor = FLinearColor::Black;

		const FLinearColor TextColor;
		const FLinearColor BorderColor;
		const FLinearColor BackgroundColor;
		const FSlateFontInfo Font;

		FScreenStyle()
			: TextColor(GetDefault<USusiSettings>()->TextColor)
			, BorderColor(GetDefault<USusiSettings>()->BorderColor)
			, BackgroundColor(GetDefault<USusiSettings>()->BackgroundColor)
			, Font(GetDefault<USusiSettings>()->Font)
		{
		}
	};
	FSusiLoader SusiLoader;
	enum EScreenState
	{
		SS_NotLogged,
		SS_PendingData,
		SS_DataLoaded
	} ScreenState;
	FString SusiUsername;

	void ConstructInitialLoginScreen();
	void ConstructDataLoadingScreen();
	void ConstructDataDisplayScreen();
	TSharedRef<SWidget> ConstructKeyValueRow(const FScreenStyle& ScreenStyle, const FString& Key, const FString& Value);

	void TransitTo(EScreenState State);


	FReply OnLoginButtonClicked();
};