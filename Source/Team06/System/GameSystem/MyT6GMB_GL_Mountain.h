// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase_GameLevel.h"
#include "MyT6GMB_GL_Mountain.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AMyT6GMB_GL_Mountain : public AT6GameModeBase_GameLevel
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
