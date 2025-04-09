#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"  
#include "ItemSpawner.generated.h"


class UBoxComponent;
class USceneComponent;
class AEquipableItem;
class ATriggerItem;
class AItemSpawnPoint;

UCLASS()
class TEAM06_API AItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

private:

	FTimerHandle DebugLogHandle;

	// 스폰 포인트 목록
	UPROPERTY()
	TArray<AItemSpawnPoint*> SpawnPoints;

	// 리스폰 시간
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	float RespawnTime = 3.0f;

	// 현재 스폰된 아이템 관리
	TMap<AItemSpawnPoint*, AEquipableItem*> SpawnedItems;

	// 스폰 범위 시각화
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;

	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	// 아이템 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	UDataTable* ItemDataTable;

	// 무작위 아이템 선택
	FItemSpawnRow* GetRandomItem() const;

	// 아이템 스폰 처리
	void SpawnItems();

	// 리스폰 처리
	void RespawnItem(AItemSpawnPoint* SpawnPoint);

	// 삭제 시 처리
	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedActor);
};