#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "Shoes_Item.generated.h"


UCLASS()
class TEAM06_API AShoes_Item : public ATriggerItem
{
	GENERATED_BODY()
	
protected:
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float SpeedBoost = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float JumpBoost = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float Duration = 5.0f; // 버프 지속 시간
};
