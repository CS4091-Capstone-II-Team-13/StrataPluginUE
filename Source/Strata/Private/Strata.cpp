// Copyright Epic Games, Inc. All Rights Reserved.

#include "Strata.h"
#include "StrataStyle.h"
#include "StrataCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName StrataTabName("Strata");

#define LOCTEXT_NAMESPACE "FStrataModule"

void FStrataModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FStrataStyle::Initialize();
	FStrataStyle::ReloadTextures();

	FStrataCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FStrataCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FStrataModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FStrataModule::RegisterMenus));
}

void FStrataModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FStrataStyle::Shutdown();

	FStrataCommands::Unregister();
}

void FStrataModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Hello, World!  Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FStrataModule::PluginButtonClicked()")),
		FText::FromString(TEXT("Strata.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FStrataModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FStrataCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FStrataCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStrataModule, Strata)