#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipable UMETA(DisplayName = "장착 아이템"),
	Trigger   UMETA(DisplayName = "트리거 아이템"),
};
