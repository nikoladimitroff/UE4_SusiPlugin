#pragma once

#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "SusiLoader.h"
#include "SusiComponent.generated.h"

/// A Susi component allows actors to be automatically logged into
/// Susi. You only need to provide the username to the component
/// and add the username & password to plugin's password file.
UCLASS(BlueprintType, ClassGroup=Susi, HideCategories = (ComponentReplication), Meta = (BlueprintSpawnableComponent, ShortTooltip = "A component used to log actors in Susi."))
class SUSI_API USusiComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USusiComponent(const FObjectInitializer& PCIP);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// Requests the user data to be reloaded from susi.
	UFUNCTION(BlueprintCallable, Category="Susi")
	void RequestDataReload();

	/// The username of the user to be logged in
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Susi")
	FString SusiUsername;

	/// The data loaded for the student. Note that if Status != ESusiStatus::DataReceived
	/// the rest of the data may be invalid. Wait for the OnInfoLoaded event before reading this property.
	UPROPERTY(BlueprintReadOnly)
	FStudentInfo LoadedStudentInfo;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInfoLoadedDelegate);

	/// Notifies the client when the data for the user has been loaded.
	UPROPERTY(BlueprintAssignable)
	FInfoLoadedDelegate OnInfoLoaded;

private:
	FSusiLoader SusiLoader;
};
