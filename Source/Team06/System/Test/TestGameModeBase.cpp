#include "System/Test/TestGameModeBase.h"
#include "System/Test/TestGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Controller/PlayerCharacterController.h"

//void ATestGameModeBase::PostLogin(APlayerCharacterController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//
//	ATestGameStateBase* TestGameState = GetGameState<ATestGameStateBase>();
//	if (IsValid(TestGameState) == false)
//	{
//		return;
//	}
//
//	APlayerCharacterController* NewPlayerController = Cast<APlayerCharacterController>(NewPlayer);
//	if (IsValid(NewPlayerController) == true)
//	{
//		//AlivePlayerControllers.Add(NewPlayerController);
//		//NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
//	}
//}

//void ATestGameModeBase::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//
//	APlayerCharacterController* ExitingPlayerController = Cast<APlayerCharacterController>(Exiting);
//	if (IsValid(ExitingPlayerController) == true && AlivePlayerControllers.Find(ExitingPlayerController) != INDEX_NONE)
//	{
//		AlivePlayerControllers.Remove(ExitingPlayerController);
//		DeadPlayerControllers.Add(ExitingPlayerController);
//	}
//}
//
//
//void ATestGameModeBase::BeginPlay()
//{
//	Super::BeginPlay();
//
//	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);
//
//	RemainWaitingTimeForPlaying = WaitingTime;
//	RemainWaitingTimeForEnding = EndingTime;
//}
//
//void ATestGameModeBase::OnMainTimerElapsed()
//{
//	ATestGameStateBase* DXGameState = GetGameState<ATestGameStateBase>();
//	if (IsValid(DXGameState) == false)
//	{
//		return;
//	}
//
//	//switch (MatchState)
//	switch (DXGameState->MatchState)
//	{
//	case EMatchState::None:
//		break;
//	case EMatchState::Waiting:
//	{
//		FString NotificationString = FString::Printf(TEXT(""));
//
//		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
//		{
//			NotificationString = FString::Printf(TEXT("Wait another players for playing."));
//
//			RemainWaitingTimeForPlaying = WaitingTime; // 최소인원이 안된다면 대기 시간 초기화.
//		}
//		else
//		{
//			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);
//
//			--RemainWaitingTimeForPlaying;
//		}
//
//		if (RemainWaitingTimeForPlaying <= 0)
//		{
//			NotificationString = FString::Printf(TEXT(""));
//
//			//MatchState = EMatchState::Playing;
//			DXGameState->MatchState = EMatchState::Playing;
//		}
//
//		NotifyToAllPlayer(NotificationString);
//
//		break;
//	}
//	case EMatchState::Playing:
//	{
//		// FString NotificationString = FString::Printf(TEXT("%d / %d"), AlivePlayerControllers.Num(), AlivePlayerControllers.Num() + DeadPlayerControllers.Num());
//		//
//		// NotifyToAllPlayer(NotificationString);
//		//
//		// if (AlivePlayerControllers.Num() <= 1)
//		// {
//		// 	MatchState = EMatchState::Ending;
//		// }
//
//		DXGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();
//
//		FString NotificationString = FString::Printf(TEXT("%d / %d"), DXGameState->AlivePlayerControllerCount, DXGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());
//
//		NotifyToAllPlayer(NotificationString);
//
//		if (DXGameState->AlivePlayerControllerCount <= 1)
//		{
//			DXGameState->MatchState = EMatchState::Ending;
//
//			AlivePlayerControllers[0]->ShowGameResultWidget(1);
//		}
//
//		break;
//	}
//	case EMatchState::Ending:
//	{
//		FString NotificationString = FString::Printf(TEXT("Waiting %d for returning to title."), RemainWaitingTimeForEnding);
//
//		NotifyToAllPlayer(NotificationString);
//
//		--RemainWaitingTimeForEnding;
//
//		if (RemainWaitingTimeForEnding <= 0)
//		{
//			for (auto AliveController : AlivePlayerControllers)
//			{
//				AliveController->ReturnToTitle();
//			}
//			for (auto DeadController : DeadPlayerControllers)
//			{
//				DeadController->ReturnToTitle();
//			}
//
//			MainTimerHandle.Invalidate();
//
//			FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
//			UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
//			// 만약 데디 서버가 게임 세션 서비스들과 연동되어 있다면,
//			// 이렇게 레벨을 다시 준비된 뒤 세션 서버한테 알려줌. "새로운 플레이어 들어올 수 있음."
//			// 그럼 세션 서비스는 새로운 플레이어들에게 데디 서버의 IP 주소를 전달해줘서 접속 유도.
//
//			return;
//		}
//
//		break;
//	}
//	case EMatchState::End:
//		break;
//	default:
//		break;
//	}
//}
//
//void ATestGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
//{
//	for (auto AlivePlayerController : AlivePlayerControllers)
//	{
//		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
//	}
//
//	for (auto DeadPlayerController : DeadPlayerControllers)
//	{
//		DeadPlayerController->NotificationText = FText::FromString(NotificationString);
//	}
//}
//
//void ATestGameModeBase::OnCharacterDead(APlayerCharacterController* InController)
//{
//	if (IsValid(InController) == false || AlivePlayerControllers.Find(InController) == INDEX_NONE)
//	{
//		return;
//	}
//
//	AlivePlayerControllers.Remove(InController);
//	DeadPlayerControllers.Add(InController);
//
//	ATestGameStateBase* TestGameState = GetGameState<ATestGameStateBase>();
//	if (IsValid(TestGameState) == true)
//	{
//		TestGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();
//	}
//	InController->ShowGameResultWidget(AlivePlayerControllers.Num() - 1);
//}