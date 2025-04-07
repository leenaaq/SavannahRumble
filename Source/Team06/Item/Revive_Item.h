#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "Revive_Item.generated.h"

UCLASS()
class TEAM06_API ARevive_Item : public ATriggerItem
{
	GENERATED_BODY()
	
public:
	ARevive_Item();

//protected:
	// 트리거 시 효과
	//virtual void ServerTriggerEffect_Implementation(AActor* OverlappedActor) override;
};
