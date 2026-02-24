// Copyright Epic Games, Inc. All Rights Reserved.

#include "Strata.h"
#include "StrataStyle.h"
//#include "StrataCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

//static const FName StrataTabName("Strata");

#define LOCTEXT_NAMESPACE "FStrataModule"

void FStrataModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FStrataStyle::Initialize();
	FStrataStyle::ReloadTextures();

	//FStrataCommands::Register();

	//PluginCommands = MakeShareable(new FUICommandList);

	/*PluginCommands->MapAction(
		FStrataCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FStrataModule::PluginButtonClicked),
		FCanExecuteAction());*/

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FStrataModule::RegisterMenus));
}

void FStrataModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FStrataStyle::Shutdown();

	//FStrataCommands::Unregister();
}

//void FStrataModule::PluginButtonClicked()
//{
//	// Put your "OnButtonClicked" stuff here
//	FText DialogText = LOCTEXT("PluginButtonDialogText", "Hello, World!");
//	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
//}

TSharedRef<SWidget> FStrataModule::GenerateStrataMenu()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.BeginSection("StrataActions", LOCTEXT("StrataActionsHeader", "Strata Tools"));
	//MenuBuilder.AddMenuEntry(FStrataCommands::Get().PluginAction);
	//MenuBuilder.AddMenuSeparator();

	// Just duplicate this to add more menu options, everything in the lambda will be executed upon click.
	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Hello"),
		LOCTEXT("ExtraActionTooltip", "World"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			FText DialogText = LOCTEXT("PluginButtonDialogText", "Hello, World!");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "World"),
		LOCTEXT("ExtraActionTooltip", "Hello"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
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

	/*{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FStrataCommands::Get().PluginAction, PluginCommands);
		}
	}*/

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
	/*FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FStrataCommands::Get().PluginAction));
	Entry.SetCommandList(PluginCommands);*/

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStrataModule, Strata)