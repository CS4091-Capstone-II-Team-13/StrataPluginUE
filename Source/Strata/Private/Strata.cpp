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

	// Duplicate this to add more submenus
	MenuBuilder.AddSubMenu(
		LOCTEXT("ExtraActionLabel", "Branch Options"),
		LOCTEXT("ExtraActionTooltip", "Change the current branch options"),
		FNewMenuDelegate::CreateRaw(this, &FStrataModule::GenerateBranchMenu)
	);

	// Duplicate this to add more menu options
	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Hello, World!"),
		LOCTEXT("ExtraActionTooltip", "Well hello there!"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() {
			// Everything inside of this function will be executed on-click
			FText DialogText = LOCTEXT("PluginButtonDialogText", "[Temporary] World, Hello!");
			FMessageDialog::Open(EAppMsgType::Ok, DialogText);
			}
		))
	);

	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FStrataModule::GenerateBranchMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("StrataActions", LOCTEXT("StrataActionsHeader", "Current Branch: origin/{branch}"));

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ExtraActionLabel", "Change current branch"),
		LOCTEXT("ExtraActionTooltip", "Change current working branch"),
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStrataModule, Strata)