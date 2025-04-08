// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "T6EMatchState.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None        UMETA(DisplayName = "None"),
	Waiting     UMETA(DisplayName = "Waiting"),
	Playing     UMETA(DisplayName = "Playing"),
	Ending      UMETA(DisplayName = "Ending"),
	Loading		UMETA(DisplayName = "Loading"),
	End         UMETA(DisplayName = "End")
};