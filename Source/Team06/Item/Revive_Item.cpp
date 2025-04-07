#include "Item/Revive_Item.h"
#include "GameFramework/Character.h"
//#include "Player/Player/PlayerBase.h" // → LifeCount 가 정의된 캐릭터 클래스
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARevive_Item::ARevive_Item()
{
	DebugMessage = TEXT("부활 아이템이 작동했습니다.");
}

//void ARevive_Item::ServerTriggerEffect_Implementation(AActor* OverlappedActor)
//{
//	// 안전한 캐스팅
//	APlayerBase* PlayerCharacter = Cast<APlayerBase>(OverlappedActor);
//	if (PlayerCharacter)
//	{
//		// 생존 중일 경우만 부활권 지급
//		if (!PlayerCharacter->IsActorBeingDestroyed() && PlayerCharacter->IsAlive())
//		{
//			PlayerCharacter->LifeCount++;
//
//			UE_LOG(LogTemp, Warning, TEXT("[ReviveItem] %s의 부활권 증가 → 현재 LifeCount: %d"),
//				*PlayerCharacter->GetName(), PlayerCharacter->LifeCount);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("[ReviveItem] 죽은 캐릭터에게는 부활권을 지급하지 않습니다."));
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("[ReviveItem] 캐릭터가 아님."));
//	}
//}

