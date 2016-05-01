// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SusiPrivatePCH.h"
#include "SSusiLoginScreen.h"

#include "SOverlay.h"
#include "SBoxPanel.h"
#include "SEditableText.h"
#include "STextBlock.h"
#include "SThrobber.h"

#define LOCTEXT_NAMESPACE "SSusiLoginScreen"

void SSusiLoginScreen::Construct(const FArguments& Args)
{
	TransitTo(EScreenState::SS_NotLogged);
}

void SSusiLoginScreen::ConstructInitialLoginScreen()
{
	FScreenStyle ScreenStyle;

	LoginScreenData.UsernameField =
		SNew(SEditableText)
		.Text(LOCTEXT("DefaultUsername", "username"))
		.ColorAndOpacity(ScreenStyle.FillColor)
		.Font(ScreenStyle.Font);

	LoginScreenData.PasswordField =
		SNew(SEditableText)
		.IsPassword(true)
		.Text(LOCTEXT("DefaultPassword", "password"))
		.ColorAndOpacity(ScreenStyle.FillColor)
		.Font(ScreenStyle.Font);

	LoginScreenData.ErrorField =
		SNew(STextBlock)
		.Visibility(EVisibility::Collapsed)
		.ColorAndOpacity(ScreenStyle.TextColor)
		.ShadowColorAndOpacity(ScreenStyle.ShadowColor)
		.Font(ScreenStyle.Font);

	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
			.BorderBackgroundColor(ScreenStyle.BackgroundColor)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					LoginScreenData.ErrorField.ToSharedRef()
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CredentialsLabel", "Enter your login details:"))
					.ColorAndOpacity(ScreenStyle.TextColor)
					.ShadowColorAndOpacity(ScreenStyle.ShadowColor)
					.Font(ScreenStyle.Font)
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					LoginScreenData.UsernameField.ToSharedRef()
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					LoginScreenData.PasswordField.ToSharedRef()
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					SNew(SButton)
					.ButtonColorAndOpacity(ScreenStyle.TextColor)
					.Cursor(EMouseCursor::Hand)
					.OnClicked(FOnClicked::CreateSP(this, &SSusiLoginScreen::OnLoginButtonClicked))
					[
						SNew(STextBlock)
						.Text(LOCTEXT("LoginButtonMessage", "Log me in!"))
						.ColorAndOpacity(ScreenStyle.TextColor)
						.ShadowColorAndOpacity(ScreenStyle.ShadowColor)
						.Font(ScreenStyle.Font)
					]
				]
			]
		];
}

FReply SSusiLoginScreen::OnLoginButtonClicked()
{
	bool bRequestSent = SusiLoader.Login(LoginScreenData.UsernameField->GetText().ToString(), LoginScreenData.PasswordField->GetText().ToString());
	SusiUsername = LoginScreenData.UsernameField->GetText().ToString();
	if (bRequestSent)
	{
		TransitTo(EScreenState::SS_PendingData);
	}
	else
	{
		LoginScreenData.ErrorField->SetText(FString(TEXT("Request sending failed!")));
		LoginScreenData.ErrorField->SetVisibility(EVisibility::Visible);
	}
	return FReply::Handled();
}

void SSusiLoginScreen::ConstructDataLoadingScreen()
{
	FScreenStyle ScreenStyle;

	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
			.BorderBackgroundColor(ScreenStyle.BackgroundColor)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SThrobber)
				.Animate(SThrobber::EAnimation::All)
			]
		];
}

TSharedRef<SWidget> SSusiLoginScreen::ConstructKeyValueRow(const FScreenStyle& ScreenStyle, const FString& Key, const FString& Value)
{
	return SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Key))
			.ColorAndOpacity(ScreenStyle.TextColor)
			.ShadowColorAndOpacity(ScreenStyle.ShadowColor)
			.Font(ScreenStyle.Font)
		]
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(FText::FromString(Value))
			.ColorAndOpacity(ScreenStyle.TextColor)
			.ShadowColorAndOpacity(ScreenStyle.ShadowColor)
			.Font(ScreenStyle.Font)
		];
}


void SSusiLoginScreen::ConstructDataDisplayScreen()
{
	FScreenStyle ScreenStyle;

	auto StudentInfo = SusiLoader.GetStudentInfo(SusiUsername);
	check(StudentInfo);
	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
			.BorderBackgroundColor(ScreenStyle.BackgroundColor)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					ConstructKeyValueRow(ScreenStyle, TEXT("User: "), StudentInfo->Name)
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					ConstructKeyValueRow(ScreenStyle, TEXT("FN: "), StudentInfo->FacultyNumber)
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					ConstructKeyValueRow(ScreenStyle, TEXT("Programme: "), StudentInfo->Programme)
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					ConstructKeyValueRow(ScreenStyle, TEXT("Year: "), FString::FormatAsNumber(StudentInfo->Year))
				]
				+SVerticalBox::Slot().VAlign(VAlign_Center)
				[
					ConstructKeyValueRow(ScreenStyle, TEXT("Group: "), FString::FormatAsNumber(StudentInfo->Group))
				]
			]
		];
}

void SSusiLoginScreen::TransitTo(EScreenState State)
{
	switch (State)
	{
	case EScreenState::SS_NotLogged:
		ConstructInitialLoginScreen();
		break;
	case EScreenState::SS_PendingData:
		ConstructDataLoadingScreen();
		LoginScreenData.UsernameField.Reset();
		LoginScreenData.PasswordField.Reset();
		LoginScreenData.ErrorField.Reset();
		break;
	case EScreenState::SS_DataLoaded:
		ConstructDataDisplayScreen();
		break;
	}
	ScreenState = State;
}

void SSusiLoginScreen::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SusiLoader.Tick(InDeltaTime);
	auto StudentInfo = SusiLoader.GetStudentInfo(SusiUsername);
	if (StudentInfo && StudentInfo->Status == ESusiStatus::LoggedIn && ScreenState == EScreenState::SS_PendingData)
	{
		bool bRequestSent = SusiLoader.RequestStudentInfo(SusiUsername);
		if (!bRequestSent)
		{
			TransitTo(EScreenState::SS_NotLogged);
			LoginScreenData.ErrorField->SetText(FString(TEXT("Request sending failed!")));
			LoginScreenData.ErrorField->SetVisibility(EVisibility::Visible);
		}
	}
	if (StudentInfo && StudentInfo->Status == ESusiStatus::ReceivedData && ScreenState == EScreenState::SS_PendingData)
	{
		TransitTo(EScreenState::SS_DataLoaded);
	}
}

#undef LOCTEXT_NAMESPACE