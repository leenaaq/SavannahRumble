// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/PCController_Title.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void APCController_Title::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(TitleUIWidgetClass) == true)
	{
		TitleUIWidgetClassInstance = CreateWidget<UUserWidget>(this, TitleUIWidgetClass);
		if (IsValid(TitleUIWidgetClassInstance) == true)
		{
			TitleUIWidgetClassInstance->AddToViewport();

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(TitleUIWidgetClassInstance->GetCachedWidget());
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}
}

void APCController_Title::JoinServer(const FString& InIPAddress)
{
	FName NextLevelName = FName(*InIPAddress);
	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true);
}
