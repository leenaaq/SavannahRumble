#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "Banana_Item.generated.h"


UCLASS()
class TEAM06_API ABanana_Item : public ATriggerItem
{
	GENERATED_BODY()
	
protected:
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float SlideForce = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float RagdollDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Effect|Sound")
	USoundBase* SlipSound;

public:
	virtual void OnRespawned_Implementation() override;
};
