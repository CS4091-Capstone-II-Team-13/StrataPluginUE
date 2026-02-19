// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "StrataStyle.h"

class FStrataCommands : public TCommands<FStrataCommands>
{
public:

	FStrataCommands()
		: TCommands<FStrataCommands>(TEXT("Strata"), NSLOCTEXT("Contexts", "Strata", "Strata Plugin"), NAME_None, FStrataStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
