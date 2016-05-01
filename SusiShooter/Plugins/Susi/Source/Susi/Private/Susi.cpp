// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SusiPrivatePCH.h"
#include "Susi.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "SusiSettings.h"
#endif

#define LOCTEXT_NAMESPACE "FSusiModule"

class FSusiModule : public ISusiModule
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace
{
#if WITH_EDITOR
	void RegisterSettings()
	{
#define LOCTEXT_NAMESPACE "SusiSettings"
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			auto defaultValues = GetMutableDefault<USusiSettings>();
			SettingsModule->RegisterSettings("Project", "Plugins", "Susi",
				LOCTEXT("RuntimeSettingsName", "Susi"),
				LOCTEXT("RuntimeSettingsDescription", "Configure the Susi plugin"),
				defaultValues
				);
		}
#undef LOCTEXT_NAMESPACE
	}
#endif
}

void FSusiModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_EDITOR
	RegisterSettings();
#endif
}

void FSusiModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSusiModule, Susi)

// LOOK AT ME
DEFINE_LOG_CATEGORY(LogSusi);