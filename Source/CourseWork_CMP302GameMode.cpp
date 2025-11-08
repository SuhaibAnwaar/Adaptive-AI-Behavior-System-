// Copyright Epic Games, Inc. All Rights Reserved.

#include "CourseWork_CMP302GameMode.h"
#include "CourseWork_CMP302Character.h"
#include "UObject/ConstructorHelpers.h"

ACourseWork_CMP302GameMode::ACourseWork_CMP302GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
