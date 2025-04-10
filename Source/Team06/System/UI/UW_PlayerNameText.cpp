#include "System/UI/UW_PlayerNameText.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Player/Player/PlayerBase.h"

UUW_PlayerNameText::UUW_PlayerNameText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_PlayerNameText::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayerName();
}

void UUW_PlayerNameText::UpdatePlayerName()
{
	APlayerBase* PC = Cast<APlayerBase>(OwningActor);
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUW_PlayerNameText : UpdatePlayerName - OwningActor is not a valid APlayerBase."));
		return;
	}

	if (IsValid(PC->GetPlayerState()) == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UUW_PlayerNameText : UpdatePlayerName - PlayerState is invalid."));
		return;
	}

	FString PlayerName = PC->GetPlayerState()->GetPlayerName();
	if (PlayerName.IsEmpty())
	{
		PlayerName = TEXT("Guest");
	}

	PlayerNameText->SetText(FText::FromString(PlayerName));
}
