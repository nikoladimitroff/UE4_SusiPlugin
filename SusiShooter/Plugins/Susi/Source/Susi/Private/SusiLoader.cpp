// Fill out your copyright notice in the Description page of Project Settings.

#include "SusiPrivatePCH.h"
#include "SusiLoader.h"

#include "Http.h"
#include "JSONUtilities.h"

FStudentInfo::FStudentInfo()
	: Name(InvalidValue)
	, FacultyNumber(InvalidValue)
	, Username(InvalidValue)
	, SusiApiKey(InvalidValue)
	, Status(ESusiStatus::LoggedOut)
{
}
const FString FStudentInfo::InvalidValue = TEXT("INVALID");

FSusiLoader::FSusiLoader()
{
}

FSusiLoader::~FSusiLoader()
{
	// TODO: Log all users out before getting destructed
}

namespace
{
	static struct
	{
		FString LoginUrl = TEXT("http://susi.apphb.com/api/login");
		FString StudentInfoUrl = TEXT("http://susi.apphb.com/api/student");
	} SusiPaths;

	FString GetUsernameFromRequest(FHttpRequestPtr Request)
	{
		return Request->GetHeader("X-USER");
	}
}

bool FSusiLoader::Login(const FString& Username, const FString& Password)
{
	auto& HttpModule = FHttpModule::Get();
	auto Request = HttpModule.CreateRequest();

	Request->OnProcessRequestComplete().BindRaw(this, &FSusiLoader::OnRequestComplete);

	FStringFormatArg UsernameArg(Username);
	FStringFormatArg PasswordArg(Password);
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(UsernameArg);
	FormatArgs.Add(PasswordArg);

	FString RequestBody = FString::Format(TEXT("{ username: '{0}', password: '{1}' }"), FormatArgs);

	TArray<uint8> RawRequestBody;
	FTCHARToUTF8 AsUtf8(*RequestBody);
	RawRequestBody.SetNumUninitialized(AsUtf8.Length());
	FMemory::Memcpy(RawRequestBody.GetData(), AsUtf8.Get(), RawRequestBody.Num());

	Request->SetURL(SusiPaths.LoginUrl);
	Request->SetHeader("X-USER", Username);
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContent(RawRequestBody);
	Request->SetVerb("POST");

	bool bDidFire = Request->ProcessRequest();
	if (bDidFire)
	{
		PendingRequests.Add(Request);
		FStudentInfo UserInfo;
		UserInfo.Username = Username;
		UserInfo.Status = ESusiStatus::PendingLogin;
		Users.Add(UserInfo.Username, UserInfo);
	}
	return bDidFire;
}

bool FSusiLoader::RequestStudentInfo(const FString& Username)
{
	auto& UserInfo = Users.FindChecked(Username);
	check(UserInfo.SusiApiKey != FStudentInfo::InvalidValue);

	auto& HttpModule = FHttpModule::Get();
	auto Request = HttpModule.CreateRequest();

	Request->OnProcessRequestComplete().BindRaw(this, &FSusiLoader::OnRequestComplete);

	FStringFormatArg KeyArg(UserInfo.SusiApiKey);
	TArray<FStringFormatArg> FormatArgs;
	FormatArgs.Add(KeyArg);

	FString RequestBody = FString::Format(TEXT("{ key: \"{0}\" }"), FormatArgs);

	TArray<uint8> RawRequestBody;
	FTCHARToUTF8 AsUtf8(*RequestBody);
	RawRequestBody.SetNumUninitialized(AsUtf8.Length());
	FMemory::Memcpy(RawRequestBody.GetData(), AsUtf8.Get(), RawRequestBody.Num());

	Request->SetURL(SusiPaths.StudentInfoUrl);
	Request->SetHeader("X-USER", Username);
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContent(RawRequestBody);
	Request->SetVerb("POST");

	UserInfo.Status = ESusiStatus::PendingData;

	return Request->ProcessRequest();
}

const FStudentInfo* FSusiLoader::GetStudentInfo(const FString& Username) const
{
	return Users.Find(Username);
}

void FSusiLoader::Tick(float Delta)
{
	for (auto& Request : PendingRequests)
	{
		Request->Tick(Delta);
	}
}

void FSusiLoader::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	PendingRequests.Remove(Request);
	if (!bWasSuccessful)
	{
		UE_LOG(LogSusi, Error, TEXT("Request to SUSI failed becauce of %d: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
	}

	if (Request->GetURL() == SusiPaths.LoginUrl)
	{
		ProcessLoginRequest(GetUsernameFromRequest(Request), Response);
	}
	else if (Request->GetURL() == SusiPaths.StudentInfoUrl)
	{
		ProcessStudentInfoRequest(GetUsernameFromRequest(Request), Response);
	}
	else
	{
		verify(false);
	}
}


void FSusiLoader::ProcessLoginRequest(const FString& Username, FHttpResponsePtr Response)
{
	// TODO: check(Response->Status == 200)
	FString Key = Response->GetContentAsString().Replace(TEXT("\""), TEXT(""));
	auto& UserInfo = Users.FindChecked(Username);
	UserInfo.SusiApiKey = Key;
	UserInfo.Status = ESusiStatus::LoggedIn;
}

void FSusiLoader::ProcessStudentInfoRequest(const FString& Username, FHttpResponsePtr Response)
{
	TSharedPtr<FJsonObject> Content;
	auto JsonParser = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	check(FJsonSerializer::Deserialize(JsonParser, Content));
	
	auto& UserInfo = Users.FindChecked(Username);
	UserInfo.Name = Content->GetStringField("firstName") + " " + Content->GetStringField("lastName");
	UserInfo.FacultyNumber = Content->GetStringField("facultyNumber");
	UserInfo.Programme = Content->GetStringField("programme");
	UserInfo.Year = Content->GetIntegerField("year");
	UserInfo.Group = Content->GetIntegerField("group");
	
	UserInfo.Status = ESusiStatus::ReceivedData;
}