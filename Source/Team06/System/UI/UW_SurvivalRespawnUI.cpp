// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UI/UW_SurvivalRespawnUI.h"

void UUW_SurvivalRespawnUI::SetProgressBarVisibility(bool bVisible)
{
	if (bVisible)
	{
		RespawnProgressBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		RespawnProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUW_SurvivalRespawnUI::SetRespawnProgress(float Percent)
{
	if (RespawnProgressBar)
	{
		RespawnProgressBar->SetPercent(Percent);
	}
}
