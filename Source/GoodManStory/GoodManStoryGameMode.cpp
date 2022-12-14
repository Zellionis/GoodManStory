// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GoodManStoryGameMode.h"
#include "UObject/ConstructorHelpers.h"

AGoodManStoryGameMode::AGoodManStoryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character/Players/PlayerBlueprint"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
