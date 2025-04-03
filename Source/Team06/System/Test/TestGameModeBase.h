#pragma once

#include "GameFramework/GameModeBase.h"
#include "TestGameModeBase.generated.h"

class APlayerCharacterController;

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

/**
 *
 */

UCLASS()
class TEAM06_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//virtual void PostLogin(APlayerCharacterController* NewPlayer) override;

	//virtual void Logout(AController* Exiting) override;

	//virtual void BeginPlay() override;

//	void OnCharacterDead(APlayerCharacterController* InController);
//
//private:
//	UFUNCTION()
//	void OnMainTimerElapsed();
//
//	void NotifyToAllPlayer(const FString& NotificationString);
//
//public:
//	FTimerHandle MainTimerHandle;
//
//	//EMatchState MatchState = EMatchState::Waiting;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//	int32 WaitingTime = 15;
//
//	int32 RemainWaitingTimeForPlaying = 15;
//
//	int32 MinimumPlayerCountForPlaying = 2;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//	int32 EndingTime = 15;
//
//	int32 RemainWaitingTimeForEnding = 15;
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	TArray<TObjectPtr<APlayerCharacterController>> AlivePlayerControllers;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	TArray<TObjectPtr<APlayerCharacterController>> DeadPlayerControllers;

};
