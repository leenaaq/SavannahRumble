#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "TurtleShell_Item.generated.h"


UCLASS()
class TEAM06_API ATurtleShell_Item : public ATriggerItem
{
	GENERATED_BODY()
	
protected:
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float BounceImpulse = 3000.0f;
};
