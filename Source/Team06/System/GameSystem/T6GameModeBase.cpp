// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameModeBase.h"
#include "System/GameSystem/T6GameInstance.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerState/T6PlayerCharacterState_GamePlay.h"
#include "AI/System/AIC_Enemy.h"
#include "AI/Character/AICharacter.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"

void AT6GameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : SessionPlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

}

AT6GameModeBase::AT6GameModeBase()
{
	// bUseSeamlessTravel = true;
}

void AT6GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("InitGame -MapName: %s"), *MapName);
	UE_LOG(LogTemp, Warning, TEXT("InitGame -Options: %s"), *Options);

}

void AT6GameModeBase::GameWelcomePlayer(UNetConnection* Connection, FString& RedirectURL)
{
	Super::GameWelcomePlayer(Connection, RedirectURL);
	UE_LOG(LogTemp, Warning, TEXT("GameWelcomePlayer -RedirectURL: %s"), *RedirectURL);
}

void AT6GameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("PreLogin -Address: %s"), *Address);
	UE_LOG(LogTemp, Warning, TEXT("PreLogin -Options: %s"), *Options);

}

APlayerController* AT6GameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	// 1. URL 옵션에서 Name 추출
	FString DesiredPlayerName = UGameplayStatics::ParseOption(Options, TEXT("TName"));
	UE_LOG(LogTemp, Warning, TEXT("Login - Parsed PlayerName from Options: %s"), *DesiredPlayerName);

	// 이 함수는 APlayerController* 를 리턴해야 하므로, Super 를 호출
	APlayerController* NewPC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return NewPC;
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

	AT6GameStateBase* GSB = GetGameState<AT6GameStateBase>();
	if (GSB)
	{
		GSB->InitAISpawnPoint();
	}
}

FString AT6GameModeBase::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	FString IncomingName = UGameplayStatics::ParseOption(Options, TEXT("TName"));
	UE_LOG(LogTemp, Log, TEXT("Option set to '%s'"), *Options);
	if (NewPlayerController && NewPlayerController->PlayerState)
	{
		NewPlayerController->PlayerState->SetPlayerName(IncomingName);
		UE_LOG(LogTemp, Log, TEXT("PlayerName set to '%s' via InitNewPlayer"), *IncomingName);
	}
	FString IsClient = UGameplayStatics::ParseOption(Options, TEXT("Client"));
	if (IsClient == FString("true"))
	{
		// 실제 쿼리로 ?Client=true를 받은 접속 요청에 대해서만 등록 시작
		UT6GameInstance* GI = Cast<UT6GameInstance>(GetGameInstance());


		GI->RegisterPlayer(IncomingName);
		UE_LOG(LogTemp,Warning,TEXT("Server-GameMode: Player %s Registered"), *IncomingName);
	}

	return Result;
}

void AT6GameModeBase::SetPlayerReady(APCController_GamePlay* PlayerController, bool bIsReady)
{
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

void AT6GameModeBase::SpawnAIControllers()
{
	if(!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("UnExpected AISpawning at GameModeBase"));
		return;
	}
	else
	{
		UT6GameInstance* GI = GetGameInstance<UT6GameInstance>();
		if (GI)
		{
			if (!GI->bIsAIAvailable)
			{
				UE_LOG(LogTemp, Error, TEXT("No_AI_Allowed"));
			}
			else
			{
				int32 SpawnIndex = 0;
				for (TTuple<FString, FPlayerScore> AIPlayer : GI->AIScoreBoard)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					AT6GameStateBase* GSB = GetGameState<AT6GameStateBase>();
					if (!IsValid(GSB))
					{
						UE_LOG(LogTemp, Error, TEXT("AISpawnPoints is empty or invalid GameState"));
						return;
					}


					FVector SpawnLocation = GSB->AISpawnPoints[SpawnIndex]->GetActorLocation();
					FRotator SpawnRotation = GSB->AISpawnPoints[SpawnIndex]->GetActorRotation();
					SpawnIndex++;

					AAIController* NewAI = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnLocation, SpawnRotation, SpawnParams);
					UE_LOG(LogTemp, Warning, TEXT("AI Controller Spawned!!!"));
					if (NewAI)
					{
						AAICharacter* AIPawn = GetWorld()->SpawnActor<AAICharacter>(AICharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
						UE_LOG(LogTemp, Warning, TEXT("AI Pawn Spawned!!!"));
						
						if (AIPawn)
						{
							NewAI->Possess(AIPawn);

							UE_LOG(LogTemp, Warning, TEXT("AI Possessed!!!"));

							if (NewAI->PlayerState)
							{
								NewAI->PlayerState->SetPlayerName(AIPlayer.Key);
								UE_LOG(LogTemp, Warning, TEXT("AI PlayerName Setting!!!"));
							}

						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("AIPawn InValid!!!"));
						}
					}
				}

			}
		}
	}
}
