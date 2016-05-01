// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "SusiLoader.generated.h"

UENUM()
enum class ESusiStatus : uint8
{
	LoggedOut,
	PendingLogin,
	LoggedIn,
	PendingData,
	ReceivedData
};

USTRUCT(BlueprintType)
struct SUSI_API FStudentInfo
{
	GENERATED_USTRUCT_BODY()

	FStudentInfo();

	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	FString FacultyNumber;
	UPROPERTY(BlueprintReadOnly)
	FString Programme;
	UPROPERTY(BlueprintReadOnly)
	uint8 Year;
	UPROPERTY(BlueprintReadOnly)
	uint8 Group;

	UPROPERTY(BlueprintReadOnly)
	FString Username;
	UPROPERTY(BlueprintReadOnly)
	FString SusiApiKey;
	UPROPERTY(BlueprintReadOnly)
	ESusiStatus Status;

	static const FString InvalidValue;
};

class SUSI_API FSusiLoader
{
public:
	FSusiLoader();
	~FSusiLoader();
	bool Login(const FString& Username, const FString& Password);
	bool RequestStudentInfo(const FString& Username);
	const FStudentInfo* GetStudentInfo(const FString& Username) const;
	void Tick(float Delta);

private:
	TMap<FString, FStudentInfo> Users; // Username to Student

	typedef TSharedPtr<class IHttpRequest> FHttpRequestPtr;
	typedef TSharedPtr<class IHttpResponse, ESPMode::ThreadSafe> FHttpResponsePtr;
	TArray<FHttpRequestPtr> PendingRequests;

	void OnRequestComplete(FHttpRequestPtr, FHttpResponsePtr, bool);
	void ProcessLoginRequest(const FString&, FHttpResponsePtr);
	void ProcessStudentInfoRequest(const FString&, FHttpResponsePtr);
};
