// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "T6GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UT6GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	FString GetRandomLevel();
	void LoadLevelsFromFolder();
private:
	TArray<FString> LevelList;
};
