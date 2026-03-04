// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FStrataModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	void PluginButtonClicked();

private:
	TSharedRef<SWidget> GenerateStrataMenu();
	void GenerateBranchMenu(FMenuBuilder& MenuBuilder);
	FReply OnCreateBranchConfirmed(TSharedPtr<SWindow> InputWindow, FString NewBranchName);

	void RegisterMenus();
	void OpenSettingsFile();

};
