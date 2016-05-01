#include "SusiPrivatePCH.h"
#include "SusiComponent.h"
#include "SusiSettings.h"



USusiComponent::USusiComponent(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_StartPhysics;
}

void USusiComponent::BeginPlay()
{
	auto PasswordFilePath = FPaths::GameContentDir() / GetDefault<USusiSettings>()->PasswordFile;
	TArray<FString> PasswordFileContents;
	if (!FFileHelper::LoadANSITextFileToStrings(*PasswordFilePath, nullptr, PasswordFileContents))
	{
		UE_LOG(LogSusi, Error,
			TEXT("Failed to load password file! Actor '%s' with username '%s' won't be able to log in Susi"),
			*GetOwner()->GetName(), *SusiUsername);
		return;
	}
	for (auto& Line : PasswordFileContents)
	{
		int32 Index;
		Line.FindChar(',', Index);
		check(Index != INDEX_NONE);
		auto LineUsername = Line.Left(Index);
		if (LineUsername == SusiUsername)
		{
			auto LinePassword = Line.RightChop(Index + 1);
			SusiLoader.Login(SusiUsername, LinePassword);
			return;
		}
	}
}

void USusiComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if all the susi data has been received. If it hasn't - tick the susi loader
	// and possibly request new info
	if (LoadedStudentInfo.Status != ESusiStatus::ReceivedData)
	{
		SusiLoader.Tick(DeltaTime);
		auto StudentInfo = SusiLoader.GetStudentInfo(SusiUsername);
		if (StudentInfo && StudentInfo->Status == ESusiStatus::LoggedIn)
		{
			SusiLoader.RequestStudentInfo(SusiUsername);
		}
		else if (StudentInfo && StudentInfo->Status == ESusiStatus::ReceivedData)
		{
			LoadedStudentInfo = *StudentInfo;
			OnInfoLoaded.Broadcast();
		}
	}
}

void USusiComponent::RequestDataReload()
{
	// Reset the status to LoggedIn; the TickComponent function will do the rest
	LoadedStudentInfo.Status = ESusiStatus::LoggedIn;
}
