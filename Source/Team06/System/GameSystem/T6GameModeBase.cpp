// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameModeBase.h"
#include "System/GameSystem/T6GameInstance.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerState/T6PlayerCharacterState_GamePlay.h"
void AT6GameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : SessionPlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

}
APlayerController* AT6GameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole,Portal, Options, UniqueId, ErrorMessage);

	FString Name = UGameplayStatics::ParseOption(Options, "Name");
	PC->PlayerState->SetPlayerName(Name);

	return PC;
}
void AT6GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AT6GameStateBase* T6GameState = GetGameState<AT6GameStateBase>();
	if (IsValid(T6GameState) == false)
	{
		return;
	}
	APCController_GamePlay* NewPlayerController = Cast<APCController_GamePlay>(NewPlayer);
	if (IsValid(NewPlayerController) == true)
	{
		SessionPlayerControllers.Add(NewPlayerController);
		UE_LOG(LogTemp, Warning, TEXT("NewPlayer PostLogin"));
		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
	}
}
void AT6GameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APCController_GamePlay* ExitingPlayerController = Cast<APCController_GamePlay>(Exiting);
	if (IsValid(ExitingPlayerController) == true && SessionPlayerControllers.Find(ExitingPlayerController) != INDEX_NONE)
	{
		SessionPlayerControllers.Remove(ExitingPlayerController);
	}
}
void AT6GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
void AT6GameModeBase::ChangeGameLevel()
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeLevel() can only be called on the server!"));
		return;
	}
	// 게임 인스턴스를 가져와서 랜덤한 레벨 선택
	UT6GameInstance* MPGameInstance = Cast<UT6GameInstance>(GetGameInstance());
	if (!MPGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
		return;
	}
	FString NextLevel = MPGameInstance->GetRandomLevel();
	if (NextLevel.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid levels found!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Traveling to: %s"), *NextLevel);

	// 서버에서 레벨 변경 실행
	GetWorld()->ServerTravel(NextLevel, true);
}

void AT6GameModeBase::OnShowingLoadingScreen()
{
	for (auto LP : SessionPlayerControllers)
	{
		if (IsValid(LP) == true)
		{
			LP->ShowGameLoadingWidget();
		}
	}
}
