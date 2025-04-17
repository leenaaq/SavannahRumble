// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UW_SurvivalRespawnUI.generated.h"

class UProgressBar;

UCLASS()
class TEAM06_API UUW_SurvivalRespawnUI : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Respawn Progress Bar, 바인딩 위젯 */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* RespawnProgressBar;
	
	void SetProgressBarVisibility(bool bVisible);
	
	/** Blueprint에서만 필요했던 인터페이스 제거 후 C++에서 직접 제어 */
	void SetRespawnProgress(float Percent);
};
