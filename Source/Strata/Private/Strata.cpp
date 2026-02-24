// Copyright Epic Games, Inc. All Rights Reserved.

#include "Strata.h"
#include "StrataStyle.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"


#define LOCTEXT_NAMESPACE "FStrataModule"

void FStrataModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FStrataStyle::Initialize();
	FStrataStyle::ReloadTextures();

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FStrataModule::RegisterMenus));
}

void FStrataModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FStrataStyle::Shutdown();
}

TSharedRef<SWidget> FStrataModule::GenerateStrataMenu()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	// Separator for different sections
	//MenuBuilder.AddMenuSeparator();

	MenuBuilder.BeginSection("StrataActions", LOCTEXT("StrataActionsHeader", "Strata Tools"));

	// Just duplicate this to add more menu options
	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Hello"),
		LOCTEXT("ExtraActionTooltip", "World"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			// Everything inside of this function will be executed on-click
			FText DialogText = LOCTEXT("PluginButtonDialogText", "Hello, World!");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "World"),
		LOCTEXT("ExtraActionTooltip", "Hello"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			// Everything inside of this function will be executed on-click
			FText DialogText = LOCTEXT("PluginButtonDialogText", "World, Hello!");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}))
	);

	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FStrataModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);


	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

	FToolMenuEntry ComboButtonEntry = FToolMenuEntry::InitComboButton(
		"StrataComboButton",
		FUIAction(),
		FOnGetContent::CreateRaw(this, &FStrataModule::GenerateStrataMenu),
		LOCTEXT("StrataButtonLabel", "Strata"),
		LOCTEXT("StrataButtonTooltip", "Open the Strata version control menu"),
		FSlateIcon(FStrataStyle::GetStyleSetName(), "Strata.PluginAction")
	);

	Section.AddEntry(ComboButtonEntry);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStrataModule, Strata)