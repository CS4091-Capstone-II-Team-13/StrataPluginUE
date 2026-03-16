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

	// Duplicate this to add more menu buttons
	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Add/Remove"),
		LOCTEXT("ExtraActionTooltip", "Add or remove files for the current change."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			// Everything inside of this function will be executed on-click
			FText DialogText = LOCTEXT("PluginButtonDialogText", "[Temporary] This is where the add/remove menu would pop up.");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Commit"),
		LOCTEXT("ExtraActionTooltip", "Review your changes and commit them."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			FText DialogText = LOCTEXT("PluginButtonDialogText", "[Temporary] This is where the commit menu would pop up.");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Push"),
		LOCTEXT("ExtraActionTooltip", "Review and push your committed changes."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			FText DialogText = LOCTEXT("PluginButtonDialogText", "[Temporary] This is where the push menu would pop up.");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	// Duplicate this to add more submenus
	MenuBuilder.AddSubMenu(
		LOCTEXT("ExtraActionLabel", "Branch Options"),
		LOCTEXT("ExtraActionTooltip", "Change the current branch options"),
		FNewMenuDelegate::CreateRaw(this, &FStrataModule::GenerateBranchMenu)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Open CLI..."),
		LOCTEXT("ExtraActionTooltip", "Open Strata's Command Line Interface."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FStrataModule::OpenCLI))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Settings..."), // Buttons that open external programs should end with "..."
		LOCTEXT("ExtraActionTooltip", "Open Settings via a text editor."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FStrataModule::OpenSettingsFile))
	);

	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FStrataModule::GenerateBranchMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("StrataActions", LOCTEXT("StrataActionsHeader", "Current Branch: origin/{branch}"));

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Change current branch"),
		LOCTEXT("ExtraActionTooltip", "Change current working branch."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			// Everything inside of this function will be executed on-click
			FText DialogText = LOCTEXT("PluginButtonDialogText", "[Temporary] This is where you'd change the current working branch");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Create new branch"),
		LOCTEXT("ExtraActionTooltip", "Create new working branch"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([this]() {
			// Everything inside of this function will be executed on-click
			TSharedPtr<SWindow> InputWindow = SNew(SWindow)
				.Title(LOCTEXT("InputTitle", "Create new branch"))
				.SizingRule(ESizingRule::Autosized)
				.SupportsMinimize(false)
				.SupportsMaximize(false)
				.HasCloseButton(true);

			TSharedPtr<SEditableTextBox> TextBox;

			InputWindow->SetContent(
				SNew(SVerticalBox)
				// Enter text
				+ SVerticalBox::Slot().AutoHeight().Padding(10)
				[
					SNew(STextBlock).Text(LOCTEXT("Prompt", "Enter the name of your new branch:"))
				]
				// Textbox
				+ SVerticalBox::Slot().AutoHeight().Padding(10)
				[
					// SAssignNew here as we need to be able to reference this text box in other functions
					SAssignNew(TextBox, SEditableTextBox)
						.HintText(LOCTEXT("Hint", "New branch name..."))
						.OnTextCommitted_Lambda([this, &InputWindow](const FText& TextBoxResponse, ETextCommit::Type CommitType) {
						if (CommitType == ETextCommit::OnEnter) {
							this->OnCreateBranchConfirmed(InputWindow, TextBoxResponse.ToString());
						}
							})
				]
			// OK and CANCEL buttons
			+ SVerticalBox::Slot().AutoHeight().Padding(10).HAlign(HAlign_Right)
				[
					SNew(SHorizontalBox)
						// OK button
						+ SHorizontalBox::Slot().AutoWidth().Padding(0, 0, 10, 0)
						[
							SNew(SButton)
								.Text(LOCTEXT("OK", "OK"))
								.IsFocusable(true)
								// Uses Unreal Engine's default accent blue color
								.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
								.ButtonColorAndOpacity(FAppStyle::Get().GetSlateColor("Colors.AccentBlue"))
								.OnClicked_Lambda([this, InputWindow, TextBox]() {
								// On OK click, confirm branch creation request
								OnCreateBranchConfirmed(InputWindow, TextBox->GetText().ToString());
								return FReply::Handled();
									})
						]
					// CANCEL button
					+ SHorizontalBox::Slot().AutoWidth()
						[
							SNew(SButton)
								.Text(LOCTEXT("CANCEL", "CANCEL"))
								.OnClicked_Lambda([InputWindow]() {
								// On CANCEL click, close window
								InputWindow->RequestDestroyWindow();
								return FReply::Handled();
									})
						]
				]
				);

			// Auto-focus onto text box upon window creation
			InputWindow->SetWidgetToFocusOnActivate(TextBox);

			// Create pop-up window
			FSlateApplication::Get().AddModalWindow(InputWindow.ToSharedRef(), nullptr);
			}
		))
	);
}

FReply FStrataModule::OnCreateBranchConfirmed(TSharedPtr<SWindow> InputWindow, FString NewBranchName)
{
	// TODO: If branch already exists, prompt user asking if they want to overwrite (delete and re-add) or cancel
	// TODO: Otherwise, create the new branch and switch to it

	// TODO: Remove, this is temporary
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "[Temporary] Now the plugin would try to create and switch to a new branch with the name \"{0}\""),
		FText::FromString(NewBranchName)
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	InputWindow->RequestDestroyWindow();
	return FReply::Handled();
};

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

void FStrataModule::OpenCLI() {
	// TODO: This path should be changable via the settings
	FString CLIPath = TEXT("C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe");
	FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	// TODO: This should be change to 'strata help'
	FString Arguments = FString::Printf(TEXT("-NoExit -Command \"git help; cd '%s'\""), *ProjectDir);

	FPlatformProcess::LaunchFileInDefaultExternalApplication(*CLIPath, *Arguments);
}

// TODO: Need to move the "create file if it doesn't exist" logic to a more appropriate place, this is just temporary for testing purposes
void FStrataModule::OpenSettingsFile() {
	FString SettingsPath = FPaths::ProjectPluginsDir() / TEXT("StrataPluginUE/Resources/StrataSettings.json");

	FString AbsPath = FPaths::ConvertRelativePathToFull(SettingsPath);
	FPaths::NormalizeFilename(AbsPath);

	// Create file if it doesn't exist
	if (!FPaths::FileExists(AbsPath))
	{
		FString DefaultSettings = TEXT("{\n  \"cliPath\": \"\",\n  \"server\": \"\",\n  \"user\": \"\"\n}\n");
		FFileHelper::SaveStringToFile(DefaultSettings, *AbsPath);
	}

	FPlatformProcess::LaunchFileInDefaultExternalApplication(*AbsPath);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStrataModule, Strata)