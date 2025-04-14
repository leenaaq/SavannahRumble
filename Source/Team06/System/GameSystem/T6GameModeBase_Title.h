// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "T6GameModeBase_Title.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AT6GameModeBase_Title : public AT6GameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void SetPlayerReady(APCController_GamePlay* PlayerController, bool bIsReady) override;
};
