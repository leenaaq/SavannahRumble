#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Sticky_Item.generated.h"

UCLASS()
class TEAM06_API ASticky_Item : public AEquipableItem
{
	GENERATED_BODY()

public:
	ASticky_Item();

protected:
	virtual void BeginPlay() override;

	// 충돌 감지용 스피어
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SlowArea;

	// 슬로우 적용 시간 및 효과
	UPROPERTY(EditDefaultsOnly, Category = "StickyEffect")
	float SlowDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "StickyEffect")
	float SlowMultiplier = 0.5f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};