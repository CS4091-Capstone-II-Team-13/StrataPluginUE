// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrataCommands.h"

#define LOCTEXT_NAMESPACE "FStrataModule"

void FStrataCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Strata", "Execute Strata action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
