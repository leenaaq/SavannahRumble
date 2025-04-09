#pragma once

#include "CoreMinimal.h"
#include "Item/EquipableItem.h"
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

	// 착지 후 효과 발동
	virtual void OnItemLanded_Implementation() override;

	// 아이템 사용
	virtual void ServerUseItem_Implementation(AActor* Target) override;

	// 슬로우 트랩 발동 처리
	void ActivateStickyTrap();

	//슬로우 영역 충돌
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 안전하게 아이템을 제거하는 함수
	void SafeDestroy();

	// 충돌 감지용 스피어
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SlowArea;

	// 슬로우 적용 시간 및 효과
	UPROPERTY(EditDefaultsOnly, Category = "StickyEffect")
	float SlowDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "StickyEffect")
	float SlowMultiplier = 0.5f;

	// 삭제 지연 시간 (착지 후 일정 시간 후 제거)
	UPROPERTY(EditDefaultsOnly, Category = "StickyEffect")
	float DestroyDelay = 5.0f;

	// 중복 Destroy 방지용
	UPROPERTY()
	bool bIsDestroyed = false;

	
	bool bIsActivated = false;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
	bool bDebugDrawArea = true;

	// 아이템 제거 타이머 핸들
	FTimerHandle DestroyTimerHandle;
};