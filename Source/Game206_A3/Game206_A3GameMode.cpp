// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game206_A3GameMode.h"
#include "Game206_A3Character.h"
#include "UObject/ConstructorHelpers.h"

AGame206_A3GameMode::AGame206_A3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
