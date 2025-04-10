// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/PCController_Title.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "GenericPlatform/GenericPlatformHttp.h"
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

void APCController_Title::JoinServer(const FString& InIPAddress, const FString& InUserName)
{

	//// URL 쿼리 형식으로 이름 파라미터 추가
	//FString Options = FString::Printf(TEXT("?Name=%s"), *InUserName);

	//FName NextLevelName = FName(*InIPAddress);
	//UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true, Options);

	FString EncodedName = FGenericPlatformHttp::UrlEncode(InUserName);// 공백제거로 URL화
	FString TravelURL = FString::Printf(TEXT("%s?Name=%s"), *InIPAddress, *EncodedName);
	this->PlayerState->SetPlayerName(InUserName);
	ClientTravel(TravelURL, TRAVEL_Absolute);

}
